//
//  ViewController.m
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#import "ViewController.h"
#import "MazeView.h"
#import "MazeCreator.h"
#import <QuartzCore/CALayer.h>
#import "CommonTypedef.h"

@interface ViewController () 

@property (weak, nonatomic) IBOutlet MazeView *mazeView;
@property (weak, nonatomic) IBOutlet UIImageView *resultView;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
}


- (IBAction)buttonPushed:(id)sender {
    //UIImagePickerControllerを生成
    UIImagePickerController *pickerController =[[UIImagePickerController alloc] init];
    
    //画像取得先を設定
    pickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
    
    pickerController.delegate = self;
    pickerController.allowsEditing = YES;
    
    [self presentViewController:pickerController animated:YES completion:nil];
}

// http://tmdat.tumblr.com/
- (cv::Mat)MatFromUIImage:(UIImage *)image
{
    CGImageRef imageRef = image.CGImage;
    cv::Mat mat = cv::Mat( 256, 256, CV_8UC4 );
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef contextRef;
    contextRef = CGBitmapContextCreate(mat.data,
                                       mat.cols,
                                       mat.rows,
                                       8,
                                       mat.step,  //変更しました．
                                       colorSpace,
                                       kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault);
    CGContextDrawImage(contextRef,
                       CGRectMake(0, 0, 256, 256),
                       imageRef);
    CGContextRelease(contextRef);
    CGColorSpaceRelease(colorSpace);
    
    cv::Mat ret = cv::Mat( 256, 256, CV_8UC3 );
    cv::cvtColor(mat, ret, CV_RGBA2BGR);  //変更しました．
    return ret;
}

- (UIImage *)UIImageFromMatBGR:(cv::Mat)image {
    NSData *data = [NSData dataWithBytes:image.data length:image.elemSize()*image.total()];
    CGColorSpaceRef colorSpace;
    if (image.elemSize() == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
    } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
    }
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    // Creating CGImage from cv::Mat
    CGImageRef imageRef = CGImageCreate(image.cols,                                 //width
                                        image.rows,                                 //height
                                        8,                                          //bits per component
                                        8 * image.elemSize(),                       //bits per pixel
                                        image.step.p[0],                            //bytesPerRow
                                        colorSpace,                                 //colorspace
                                        kCGImageAlphaNone|kCGBitmapByteOrderDefault,// bitmap info
                                        provider,                                   //CGDataProviderRef
                                        NULL,                                       //decode
                                        false,                                      //should interpolate
                                        kCGRenderingIntentDefault                   //intent
                                        );
    // Getting UIImage from CGImage
    UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    return finalImage;
}

static bool less_than(const std::pair<int, int>& a, const std::pair<int, int>& b){
    return a.first*a.first + a.second*a.second < b.first*b.first + b.second*b.second;
}

- (void)imagePickerController:(UIImagePickerController*)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    
    // 90度回転されているので、元に戻す
    UIGraphicsBeginImageContext(image.size);
    [image drawInRect:CGRectMake(0, 0, image.size.width, image.size.height)];
    image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    cv::Mat matImage = [self MatFromUIImage:image];
    
    // グレイスケール化
    cv::Mat grayimg, graying_tmp;
    cv::cvtColor(matImage, grayimg, CV_BGR2GRAY);
    
    
    // 二値化
    cv::Mat binimg;
//    cv::threshold(grayimg, binimg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    // 大津の方法よりこちらの方がうまく画像を認識出来た
    cv::adaptiveThreshold(grayimg, binimg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, 8);
    
    std::vector<std::pair<int, int> > mass;
    for (int y = 0; y < binimg.rows; ++y) {
        for (int x = 0; x < binimg.cols; ++x) {
            int val = static_cast<int>(binimg.at<unsigned char>(y, x));
            if (val < 200) {
//                printf("%d, %d\n", x/4, y/4);
                mass.push_back(std::make_pair(x/(256/MASS_SIZE_X), y/(256/MASS_SIZE_Y)));
            }
        }
    }
    
    std::sort(mass.begin(), mass.end(), less_than);
    mass.erase(std::unique(mass.begin(), mass.end()), mass.end());
    
    [self dismissViewControllerAnimated:YES completion:nil];
    
//    self.resultView.image = [self UIImageFromMatBGR:binimg];
//    [self.resultView setNeedsDisplay];
    
    std::vector<MassIndex> massIndex;
    for (std::vector<std::pair<int, int> >::const_iterator it = mass.begin(); it != mass.end(); ++it) {
        MassIndex m;
        m.x = it->first;
        m.y = it->second;
        massIndex.push_back(m);
    }
    
    MazeCreator::Instance().SetPath(massIndex);
    MazeCreator::Instance().Solve();
    
    [self.mazeView setMazeCreator:&MazeCreator::Instance()];
    [self.mazeView setNeedsDisplay];
    
    [self saveImage];
}

//画像を保存する
-(void)saveImage
{
    CGRect screenRect = self.mazeView.frame;
    UIGraphicsBeginImageContextWithOptions(self.mazeView.frame.size, NO, 0);
    
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    [[UIColor blackColor] set];
    CGContextFillRect(ctx, screenRect);
    
    // 保存するビューを指定
    [self.mazeView.layer renderInContext:ctx];
    
    // 指定したビューをPNGで取得
    NSData *pngData = UIImagePNGRepresentation(UIGraphicsGetImageFromCurrentImageContext());
    UIImage *screenImage = [UIImage imageWithData:pngData] ;
    // 取得したPNG画像をカメラロールに保存する
    UIImageWriteToSavedPhotosAlbum(screenImage, nil, nil, nil);
    UIGraphicsEndImageContext();

}

@end
