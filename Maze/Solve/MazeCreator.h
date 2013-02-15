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

struct MathIndex{
    int x;// 0始まりの行番号
    int y;// 0始まりの列番号
};

// 壁を除いて０始まりで境界を示す。true:壁有り、false:壁なし
typedef std::vector<bool> BoarderType;

class MazeCreator{
public:
    // 縦・横のマスサイズ
    static MazeCreator& create(int x, int y);
    
    // 予め正解を作る場合のパス
    void SetPath( const std::vector< MathIndex >& path) const;
    
    // 迷路を解く
    void Solve();
    
    // 迷路の結果を取得
    void Result( std::vector<BoarderType>& boarder_h, std::vector<BoarderType>& boarder_v ) const;
private:
    MazeCreator();
    virtual ~MazeCreator();
    
    struct Body;
    Body& m;
};


#endif /* defined(__Maze__MazeCreator__) */
