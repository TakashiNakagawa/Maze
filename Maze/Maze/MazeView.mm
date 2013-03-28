//
//  MazeView.m
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#import "MazeView.h"
#include "MazeCreator.h"
#include <vector>

@interface MazeView(){
    const MazeCreator* _maze;
    float _x_length;
    float _y_length;
}
@end


static float kRation = 0.9;

@implementation MazeView


- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        _frameColor = [UIColor redColor];
        _lineColor = [UIColor blackColor];
    }
    return self;
}


// 指定した頂点の位置を取得（左上を(0, 0)とする)
static std::pair<int, int> calcPosition(CGSize size, int x_index, int y_index, float x_length, float y_length){
    int base = size.width * (1 - kRation)/2;
    int x = base + x_index * x_length;
    int y = base + y_index * y_length;
    return std::pair<int, int>(x, y);
}

// マスの中央の位置を取得
static std::pair<float, float> calcCenterPosition(CGSize size, int x_index, int y_index, float x_length, float y_length){
    std::pair<int, int> p0 = calcPosition(size, x_index, y_index, x_length, y_length);
    std::pair<int, int> p1 = calcPosition(size, x_index+1, y_index+1, x_length, y_length);
    return std::pair<float, float>((p0.first + p1.first)*0.5, (p0.second + p1.second)*0.5);
}

-(void) setMazeCreator:(const MazeCreator*) maze {
    _maze = maze;
}

// 迷路の枠線
- (void)drawFrameWithContext:(CGContextRef) ctx
{
    CGContextSetLineWidth(ctx, 2.0);
    CGContextSetStrokeColorWithColor(ctx, self.frameColor.CGColor);
    
    std::pair<int, int> p0 = calcPosition(self.frame.size, 0, 0, _x_length, _y_length);
    std::pair<int, int> p1 = calcPosition(self.frame.size, MASS_SIZE_X, 0, _x_length, _y_length);
    std::pair<int, int> p2 = calcPosition(self.frame.size, MASS_SIZE_X, MASS_SIZE_Y, _x_length, _y_length);
    std::pair<int, int> p3 = calcPosition(self.frame.size, 0, MASS_SIZE_Y, _x_length, _y_length);
    CGContextMoveToPoint(ctx, p0.first + 2, p0.second); //start at this point
    CGContextAddLineToPoint(ctx, p1.first, p1.second); //draw to this point
    CGContextAddLineToPoint(ctx, p2.first, p2.second - 2); //draw to this point
    CGContextStrokePath(ctx);
    
    CGContextMoveToPoint(ctx, p2.first - 2, p2.second); //start at this point
    CGContextAddLineToPoint(ctx, p3.first, p3.second); //draw to this point
    CGContextAddLineToPoint(ctx, p0.first, p0.second + 2); //draw to this point
    
    // and now draw the Path!
    CGContextStrokePath(ctx);    
}

// 迷路の区切り縦線
- (void)drawVerticalBoarderWithContext:(CGContextRef)ctx atCol:(int)col atRow:(int)row 
{
    CGContextSetStrokeColorWithColor(ctx, self.lineColor.CGColor);
    CGContextSetLineWidth(ctx, 1.0);
    
    std::pair<int, int> p0 = calcPosition(self.frame.size, col, row, _x_length, _y_length);
    std::pair<int, int> p1 = calcPosition(self.frame.size, col, row+1, _x_length, _y_length);
    CGContextMoveToPoint(ctx, p0.first, p0.second); //start at this point
    CGContextAddLineToPoint(ctx, p1.first, p1.second); //draw to this point
    
    // and now draw the Path!
    CGContextStrokePath(ctx);
}

// 迷路の区切り横線
- (void)drawHorizontalBoarderWithContext:(CGContextRef)ctx atCol:(int)col atRow:(int)row
{
    CGContextSetLineWidth(ctx, 1.0);
    CGContextSetStrokeColorWithColor(ctx, self.lineColor.CGColor);
    
    std::pair<int, int> p0 = calcPosition(self.frame.size, col, row, _x_length, _y_length);
    std::pair<int, int> p1 = calcPosition(self.frame.size, col+1, row, _x_length, _y_length);
    CGContextMoveToPoint(ctx, p0.first, p0.second); //start at this point
    CGContextAddLineToPoint(ctx, p1.first, p1.second); //draw to this point
    
    // and now draw the Path!
    CGContextStrokePath(ctx);
}

// 番号の表示
- (void)drawNumber:(int)num atCol:(int)col atRow:(int)row
{
    std::pair<int, int> p = calcCenterPosition(self.frame.size, col, row, _x_length, _y_length);
    NSString *string = [NSString stringWithFormat:@"%d", num];    
    [string drawAtPoint:CGPointMake(p.first, p.second) withFont:[UIFont systemFontOfSize:8]];
}

// 迷路を表示
- (void)drawRect:(CGRect)rect
{
    [super drawRect:rect];
    
    if (!_maze) {
        return ;
    }
    
    CGSize size = self.frame.size;
    _x_length = (size.width / MASS_SIZE_X) * kRation;
    _y_length = (size.height / MASS_SIZE_Y) * kRation;
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    [self drawFrameWithContext:context];
    
    // 迷路の結果を取得
    std::vector<BoarderType> boarder_h, boarder_v;
    _maze->Result(boarder_h, boarder_v);
    
    // 縦線
    for (int i = 0; i < boarder_v.size(); ++i) {
        for (int j = 0; j < boarder_v[i].size(); ++j){
            if (!boarder_v[i][j]) {
                continue;
            }
            [self drawVerticalBoarderWithContext:context atCol:i+1 atRow:j];
        }
    }
    
    // 横線
    for (int i = 0; i < boarder_h.size(); ++i) {
        for (int j = 0; j < boarder_h[i].size(); ++j){
            if (!boarder_h[i][j]) {
                continue;
            }
            [self drawHorizontalBoarderWithContext:context atCol:j atRow:i+1];
        }
    }
    
#if false
    
    // クラスター番号の表示
    for (int x = 0; x < MASS_SIZE_X; ++x) {
        for (int y = 0; y < MASS_SIZE_Y; ++y) {
            int num = maze.ClusterNumber(x, y);
            [self drawNumber:num atCol:x atRow:y];
        }
    }
#endif
}

@end
