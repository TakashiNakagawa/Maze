//
//  CommonTypedef.h
//  Maze
//
//  Created by 中川 高志 on 2013/03/26.
//  Copyright (c) 2013年 CFlat. All rights reserved.
//

#ifndef Maze_CommonTypedef_h
#define Maze_CommonTypedef_h

//#define MASS_SIZE_X 64
//#define MASS_SIZE_Y 64
#define MASS_SIZE_X 128
#define MASS_SIZE_Y 128

struct MassIndex{
    int x;// 0始まりの行番号
    int y;// 0始まりの列番号
};

// 壁を除いて０始まりで境界を示す。true:壁有り、false:壁なし
typedef std::vector<bool> BoarderType;


#endif
