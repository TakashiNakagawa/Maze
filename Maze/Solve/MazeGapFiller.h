//
//  MazeGapFiller.h
//  Maze
//
//  Created by 中川 高志 on 2013/03/26.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#ifndef __Maze__MazeGapFiller__
#define __Maze__MazeGapFiller__

#include <iostream>
#include "CommonTypedef.h"

// 迷路のスタート or ゴールとの間を補完
class MazeGapFiller {
public:
    MazeGapFiller(const MassIndex& stt, const MassIndex& end);
    virtual ~MazeGapFiller();
    
    void GetResult( std::vector<MassIndex>& results ) const;
    
private:
    struct Body;
    Body& m;
};

#endif /* defined(__Maze__MazeGapFiller__) */
