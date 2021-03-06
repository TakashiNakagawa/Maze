//
//  MazeView.h
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#import <UIKit/UIKit.h>

class MazeCreator;
@interface MazeView : UIView

-(void) setMazeCreator:(const MazeCreator*) maze;

@property UIColor *frameColor;// 外枠の色
@property UIColor *lineColor;// 迷路の色

@end
