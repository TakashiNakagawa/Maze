//
//  MazeView.h
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#import <UIKit/UIKit.h>
@interface MazeView : UIView

-(void) setInitPath:(const std::vector<std::pair<int, int> >&) mass;

@end
