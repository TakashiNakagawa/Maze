//
//  MazeCreator.cpp
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#include "MazeCreator.h"

struct MazeCreator::Body{
    
};



MazeCreator::MazeCreator() : m( *new Body){
    
}

MazeCreator::~MazeCreator(){
    delete &m;
}


// 縦・横のマスサイズ
MazeCreator& MazeCreator::create(int x, int y){
    static MazeCreator instance;
    return instance;
}

// 予め正解を作る場合のパス
void MazeCreator::SetPath( const std::vector< MathIndex >& path) const{
    
}

// 迷路を解く
void Solve(){
    
}

// 迷路の結果を取得
void MazeCreator::Result( std::vector<BoarderType>& boarder_h, std::vector<BoarderType>& boarder_v ) const{
    
}
