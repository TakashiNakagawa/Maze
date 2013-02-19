//
//  MazeCreator.cpp
//  Maze
//
//  Created by 中川 高志 on 2013/02/15.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#include "MazeCreator.h"
#include <assert.h>


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
        boarderDirCheck[0] = true;
        boarderDirCheck[1] = true;
    }
    
    // 壁を壊すかチェック
    bool CanConnect(const std::vector<MassIndex>& massIndex) const{
        if (massIndex.size() != 2) {
            return false;
        }
        
        int lhs = cluster[massIndex[0].x][massIndex[0].y];
        int rhs = cluster[massIndex[1].x][massIndex[1].y];
       
        if (lhs == -1 && rhs == -1) {
            return true;
        }
        if (lhs == rhs){
            return false;
        }
        
        if (lhs < 0 && rhs < 0){
            return true;
        } else if (lhs < 0 || rhs < 0){
            return false;
        }
        
        return true;
    }
                
    // 壁の両側のマス番号を取得
    std::vector<MassIndex> GetMassIndex(bool vertical, int x, int y) const{
        std::vector<MassIndex> index;
        MassIndex m, m2;
        m.x = x;
        m.y = y;
        if (vertical) {
            m2.x = x + 1;
            m2.y = y;
        } else {
            m2.x = x;
            m2.y = y + 1;
        }
        index.push_back(m);
        index.push_back(m2);
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
    
    // 選択した壁が縦か横か、壁番号
    bool Select(bool& vertical, int&x, int&y) {
        if (boarderDirCheck[0] && boarderDirCheck[1]) {
            vertical = (rand() %2);
        } else {
            vertical = boarderDirCheck[0] ? true : false;
        }
        std::vector<BoarderType> boarder;
        std::vector<std::pair<int, int>> boardIndex;
        if (vertical) {
            boarder = boarder_v;
        } else {
            boarder = boarder_h;
        }
        
        for (int i = 0; i < boarder.size(); ++i) {
            for (int j = 0; j < boarder[i].size(); ++j) {
                int x = i;
                int y = j;
                if (!vertical) {
                    std::swap(x, y);
                }
                if (!boarder[i][j]) {
                    continue;
                }
                std::vector<MassIndex> mass = this->GetMassIndex(vertical, x, y);
                if (!this->CanConnect(mass)) {
                    continue;
                }
                boardIndex.push_back(std::make_pair(x, y));
            }
        }
        if (boardIndex.empty()) {
            if (boarderDirCheck[0] == false && boarderDirCheck[1] == false) {
                return false;
            }
            if (vertical) {
                boarderDirCheck[0] = false;
            } else {
                boarderDirCheck[1] = false;
            }
            // 再帰
            return this->Select(vertical, x, y);
        }
        int sel = (rand() % MASS_SIZE_X * MASS_SIZE_Y) % boardIndex.size();
        x = boardIndex[sel].first;
        y = boardIndex[sel].second;
        return true;
    }
    
    
    // 検索して見つからなければ無効にする
    void InvalidateBoarder(bool vertical) {
        if (vertical) {
            boarderDirCheck[0] = false;
        } else {
            boarderDirCheck[1] = false;
        }
    }
    
private:
    int cluster[MASS_SIZE_X][MASS_SIZE_Y];
    // 壁情報
    std::vector<BoarderType> boarder_h;// 水平の壁
    std::vector<BoarderType> boarder_v;// 垂直の壁
    bool boarderDirCheck[2]; //[0]:縦, [1]:横
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
    
    // テストコード
    for (int i = 0; i < MASS_SIZE_X; ++i) {
        m.SetCluster(i, 0, -1);
        m.SetCluster(i, 4, -1);
        m.SetCluster(i, 8, -1);
        m.SetCluster(i, 12, -1);
        m.SetCluster(i, 16, -1);
    }
    m.SetCluster(MASS_SIZE_X-1, 0, -1);
    m.SetCluster(MASS_SIZE_X-1, 1, -1);
    m.SetCluster(MASS_SIZE_X-1, 2, -1);
    m.SetCluster(MASS_SIZE_X-1, 3, -1);
    
    m.SetCluster(0, 4, -1);
    m.SetCluster(0, 5, -1);
    m.SetCluster(0, 6, -1);
    m.SetCluster(0, 7, -1);
    
    m.SetCluster(MASS_SIZE_X-1, 8, -1);
    m.SetCluster(MASS_SIZE_X-1, 9, -1);
    m.SetCluster(MASS_SIZE_X-1, 10, -1);
    m.SetCluster(MASS_SIZE_X-1, 11, -1);
    
    m.SetCluster(0, 12, -1);
    m.SetCluster(0, 13, -1);
    m.SetCluster(0, 14, -1);
    m.SetCluster(0, 15, -1);
}


// 迷路を解く
void MazeCreator::Solve(){
    while(true){
        // ランダムで壁を選択
        bool v;
        int x, y;// 壁番号
        if (!m.Select(v, x, y)){
            break;
        }
        
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

