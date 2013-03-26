//
//  MazeCreator.h
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#ifndef __Maze__MazeCreator__
#define __Maze__MazeCreator__

#include <iostream>
#include <vector>
#include "CommonTypedef.h"


class MazeCreator{
public:
    static MazeCreator& Instance();
    virtual ~MazeCreator();    
    
    // 予め正解を作る場合のパス
    void SetPath(const std::vector< MassIndex >& path) const;
    
    // 迷路を解く
    void Solve();
    
    // 迷路の結果を取得
    void Result(std::vector<BoarderType>& boarder_h, std::vector<BoarderType>& boarder_v) const;
    
    // クラスター番号の取得
    int ClusterNumber(int x, int y) const;
    
    
private:
    MazeCreator();
    
    struct Body;
    Body& m;
};


#endif /* defined(__Maze__MazeCreator__) */
