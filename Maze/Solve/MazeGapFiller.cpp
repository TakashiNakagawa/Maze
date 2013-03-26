//
//  MazeGapFiller.cpp
//  Maze
//
//  Created by 中川 高志 on 2013/03/26.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#include "MazeGapFiller.h"
#include <deque>

// 始点と終点を結ぶ線をランダムで選ぶ（x, y成分を分けて考える）


struct MazeGapFiller::Body {
    std::deque<int> xs;
    std::deque<int> ys;
    std::vector<MassIndex> results;
    
    void Initialize(const MassIndex& stt, const MassIndex& end) {
        for (int i = stt.x; i < end.x; ++i) {
            xs.push_back(i);
        }
        for (int i = stt.y; i < end.y; ++i) {
            ys.push_back(i);
        }
        
        this->RandomSelect(stt, end);
    }
    
    void RandomSelect(const MassIndex& stt, const MassIndex& end) {
        results.push_back(stt);
        while (!xs.empty() || !ys.empty()) {
            MassIndex mass;
            mass.x = stt.x;
            mass.y = stt.y;
            if (!results.empty()) {
                mass.x = !xs.empty() ? xs.front() : end.x;
                mass.y = !ys.empty() ? ys.front() : end.y;
            }
            if (ys.empty() || (!xs.empty() && (rand() % 2 == 0))) {
                mass.x += 1;
                xs.pop_front();
            } else {
                mass.y += 1;
                ys.pop_front();
            }
            results.push_back(mass);
        }
    }
    
private:
};

MazeGapFiller::MazeGapFiller(const MassIndex& stt, const MassIndex& end) : m( *new Body ){
    m.Initialize(stt, end);
}

MazeGapFiller::~MazeGapFiller() {
    delete &m;
}


void MazeGapFiller::GetResult( std::vector<MassIndex>& results ) const {
    results.swap(m.results);
}


