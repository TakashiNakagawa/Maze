//
//  MazeCreator.cpp
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#include "MazeCreator.h"
#include <assert.h>

namespace{
    
// ランダムで壁を選択
class RandamSelector{
public:
    
    // 選択した壁が縦か横か、壁番号
    void Select(const std::vector<BoarderType>& boarder_v, const std::vector<BoarderType>& boarder_h,
                bool& vertical, int&x, int&y) const{
        vertical = (rand() %2);
        std::vector<BoarderType> boarder;
        if (vertical) {
            boarder = boarder_v;
            int sel = rand() % MASS_SIZE_X * MASS_SIZE_Y;
            int sel_org = sel;
            while(true){
                for (int i = 0; i < boarder.size(); ++i) {
                    for (int j = 0; j < boarder[i].size(); ++j) {
                        if (false == boarder[i][j]) {
                            continue;
                        }
                        --sel;
                        if (sel <= 0) {
                            x = i;
                            y = j;
                            return;
                        }
                    }
                }
                if (sel == sel_org) {
                    x = 0;
                    y = 0;
                    return;
                }
            }
        } else {
            boarder = boarder_h;
            int sel = rand() % MASS_SIZE_X * MASS_SIZE_Y;
            int sel_org = sel;
            while(true){
                for (int j = 0; j < boarder.size(); ++j) {
                    for (int i = 0; i < boarder[j].size(); ++i) {
                        if (false == boarder[j][i]) {
                            continue;
                        }
                        --sel;
                        if (sel <= 0) {
                            
                            x = i;
                            y = j;
                            
                            if (x == 14 && y == 10) {
                                printf("size : %ld", boarder_h.size());
                            }
                            
                            return;
                        }
                    }
                }
                if (sel == sel_org) {
                    x = 0;
                    y = 0;
                    return;
                }
            }
        }
    }
};
    
}// anonymous namespace

struct MazeCreator::Body{
    
    Body(){
        Initialze();
    }

    void Initialze(){
        int i = 0;
        for (int y = 0; y < MASS_SIZE_Y; ++y) {
            for (int x = 0; x < MASS_SIZE_X; ++x){
                cluster[x][y] = i++;
            }
        }
        for (int i = 0; i < MASS_SIZE_Y - 1; ++i) {
            boarder_h.push_back(BoarderType(MASS_SIZE_X, true));
        }
        
        for (int i = 0; i < MASS_SIZE_X - 1; ++i) {
            boarder_v.push_back(BoarderType(MASS_SIZE_Y, true));
        }
    }
    
    // 壁を壊すかチェック
    bool CanConnect(const std::vector<MassIndex>& massIndex ) const{
        if (massIndex.size() != 2) {
            return false;
        }
        if (cluster[massIndex[0].x][massIndex[0].y] == cluster[massIndex[1].x][massIndex[1].y]){
            return false;
        }
        return true;
    }
                
    // 壁の両側のマス番号を取得
    std::vector<MassIndex> GetMassIndex(bool vertical, int x, int y) const{
        std::vector<MassIndex> index;
        if (vertical) {
            MassIndex m, m2;
            m.x = x;
            m.y = y;
            m2.x = x + 1;
            m2.y = y;
            index.push_back(m);
            index.push_back(m2);
        } else {
            MassIndex m, m2;
            m.x = x;
            m.y = y;
            m2.x = x;
            m2.y = y + 1;
            index.push_back(m);
            index.push_back(m2);
        }
        return index;
    }
    
    int Cluster(int x, int y) const{
        assert(x < MASS_SIZE_X);
        assert(y < MASS_SIZE_Y);
        return cluster[x][y];
    }
    
    void SetCluster(int x, int y, int val){
        assert(x < MASS_SIZE_X);
        assert(y < MASS_SIZE_Y);
        cluster[x][y] = val;
    }
    
    const std::vector<BoarderType>& BoarderH() const{
        return boarder_h;
    }
    
    const std::vector<BoarderType>& BoarderV() const{
        return boarder_v;
    }
    
    bool BoarderHFlag(int x, int y) const{
        assert(x <= MASS_SIZE_X);
        assert(y < boarder_h.size());
        return boarder_h[y][x];
    }
    
    void SetBoarderHFlag(int x, int y, bool flag){
        assert(x <= MASS_SIZE_X);
        assert(y < boarder_h.size());
        boarder_h[y][x] = flag;
    }
    
    bool BoarderVFlag(int x, int y) const{
        assert(y <= MASS_SIZE_Y);
        assert(x < boarder_v.size());
        return boarder_v[x][y];
    }
    
    void SetBoarderVFlag(int x, int y, bool flag){
        assert(y <= MASS_SIZE_Y);
        assert(x < boarder_v.size());
        boarder_v[x][y] = flag;
    }
    
private:
    int cluster[MASS_SIZE_X][MASS_SIZE_Y];
    // 壁情報
    std::vector<BoarderType> boarder_h;// 水平の壁
    std::vector<BoarderType> boarder_v;// 垂直の壁
};


MazeCreator::MazeCreator() : m( *new Body){
}


MazeCreator::~MazeCreator(){
    delete &m;
}


// 縦・横のマスサイズ
MazeCreator& MazeCreator::create(){
    static MazeCreator instance;
    return instance;
}

// 予め正解を作る場合のパス
void MazeCreator::SetPath( const std::vector< MassIndex >& path) const{

}


// 迷路を解く
void MazeCreator::Solve(){
    int count = 0;
    while(1000 > count++){
        // ランダムで壁を選択
        RandamSelector selector;
        bool v;
        int x, y;// 壁番号
        selector.Select(m.BoarderV(), m.BoarderH(), v, x, y);
        std::vector<MassIndex> index = m.GetMassIndex(v, x, y);
        if (v) {
            assert(m.BoarderVFlag(x, y));
        } else {
            assert(m.BoarderHFlag(x, y));
        }
        
        // 同一のクラスターなら壁を壊さない
        if (!m.CanConnect(index)) {
            continue;
        }
        
        // 異なるクラスターの場合は壁を壊す
        if (v) {
            m.SetBoarderVFlag(x, y, false);
        } else {
            m.SetBoarderHFlag(x, y, false);
        }
        
        // 大きい方のクラスター番号は小さい方のクラスター番号に合わせる
        int c1 = m.Cluster(index[0].x, index[0].y);
        int c2 = m.Cluster(index[1].x, index[1].y);
        int big = std::max(c1, c2);
        int small = std::min(c1, c2);
        
        for (int i = 0; i < MASS_SIZE_X; ++i) {
            for (int j = 0; j < MASS_SIZE_Y; ++j) {
                if (m.Cluster(i, j) == big) {
                    m.SetCluster(i, j, small);
                }
            }
        }
    }
}

// 迷路の結果を取得
void MazeCreator::Result( std::vector<BoarderType>& boarder_h, std::vector<BoarderType>& boarder_v ) const{
    boarder_h = m.BoarderH();
    boarder_v = m.BoarderV();
}

// クラスター番号の取得
int MazeCreator::ClusterNumber(int x, int y) const{
    return m.Cluster(x, y);
}

