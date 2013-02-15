//
//  MazeView.m
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#import "MazeView.h"
#include "MazeView.h"
#include <vector>

static int mass_x = 3;
static int mass_y = 3;
static int length = 60;

@implementation MazeView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}


// 指定した頂点の位置を取得（左上を(0, 0)とする)
static std::pair<int, int> calcPostion(int x_index, int y_index){
    int base = 10;
    int x = base + x_index * length;
    int y = base + y_index * length;
    return std::pair<int, int>(x, y);
}




// 迷路の枠線
- (void)drawFrameWithContext:(CGContextRef) ctx
{
    CGContextSetLineWidth(ctx, 3.0);
    CGContextSetStrokeColorWithColor(ctx, [UIColor redColor].CGColor);
    
    std::pair<int, int> p0 = calcPostion(0, 0);
    std::pair<int, int> p1 = calcPostion(mass_x, 0);
    std::pair<int, int> p2 = calcPostion(mass_x, mass_y);
    std::pair<int, int> p3 = calcPostion(0, mass_y);
    CGContextMoveToPoint(ctx, p0.first, p0.second); //start at this point
    CGContextAddLineToPoint(ctx, p1.first, p1.second); //draw to this point
    CGContextAddLineToPoint(ctx, p2.first, p2.second); //draw to this point
    CGContextAddLineToPoint(ctx, p3.first, p3.second); //draw to this point
    CGContextAddLineToPoint(ctx, p0.first, p0.second); //draw to this point
    
    // and now draw the Path!
    CGContextStrokePath(ctx);    
}

// 迷路の区切り縦線
- (void)drawVerticalBoarderWithContext:(CGContextRef)ctx atCol:(int)col atRow:(int)row 
{
    CGContextSetLineWidth(ctx, 1.0);
    CGContextSetStrokeColorWithColor(ctx, [UIColor blueColor].CGColor);
    
    std::pair<int, int> p0 = calcPostion(col, row);
    std::pair<int, int> p1 = calcPostion(col, row+1);
    CGContextMoveToPoint(ctx, p0.first, p0.second); //start at this point
    CGContextAddLineToPoint(ctx, p1.first, p1.second); //draw to this point
    
    // and now draw the Path!
    CGContextStrokePath(ctx);
}

// 迷路の区切り横線
- (void)drawHorizontalBoarderWithContext:(CGContextRef)ctx atCol:(int)col atRow:(int)row
{
    CGContextSetLineWidth(ctx, 1.0);
    CGContextSetStrokeColorWithColor(ctx, [UIColor blueColor].CGColor);
    
    std::pair<int, int> p0 = calcPostion(col, row);
    std::pair<int, int> p1 = calcPostion(col+1, row);
    CGContextMoveToPoint(ctx, p0.first, p0.second); //start at this point
    CGContextAddLineToPoint(ctx, p1.first, p1.second); //draw to this point
    
    // and now draw the Path!
    CGContextStrokePath(ctx);
}


// 迷路を表示
- (void)drawRect:(CGRect)rect
{
    [super drawRect:rect];
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    [self drawFrameWithContext:context];
    
    // 縦線
    for (int i = 1; i < mass_x; ++i) {
        for (int j = 0; j < mass_y; ++j){
            [self drawVerticalBoarderWithContext:context atCol:i atRow:j];
        }
    }
    
    // 横線
    for (int i = 0; i < mass_x; ++i) {
        for (int j = 1; j < mass_y; ++j){
            [self drawHorizontalBoarderWithContext:context atCol:i atRow:j];
        }
    }
}

@end
