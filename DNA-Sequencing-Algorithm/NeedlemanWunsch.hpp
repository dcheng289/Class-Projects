//
//  NeedlemanWunsch.hpp
//  dnawx-mac
//
//  Created by Derek Cheng on 3/8/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#ifndef NeedlemanWunsch_hpp
#define NeedlemanWunsch_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

enum Direction : char
{
    ZERO = 0,
    UP_LEFT = 1,
    LEFT = 2,
    UP = 3
};


class NeedlemanWunsch
{
public:
    NeedlemanWunsch(const std::string& fileOne, const std::string& fileTwo);
    void RunAlgorithm();
    void SaveFile(const std::string& fileName);
    
private:
    
    std::string mSequenceA = "";
    std::string mSequenceB = "";
    std::string mHeaderA = "";
    std::string mHeaderB = "";
    
    std::string mTraceBackA;
    std::string mTraceBackB;
    
    int mScore;
    
    
    const int mMatchScore = 1;
    const int mMismatchScore = -1;
    const int mGapScore = -1;
    
    std::vector<std::vector<int>> mScoreGrid;
    std::vector<std::vector<Direction>> mDirGrid;
    
};


#endif /* NeedlemanWunsch_hpp */


