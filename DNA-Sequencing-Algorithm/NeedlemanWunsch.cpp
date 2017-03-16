//
//  NeedlemanWunsch.cpp
//  dnawx-mac
//
//  Created by Derek Cheng on 3/8/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "NeedlemanWunsch.hpp"
#include "FileParser.hpp"




NeedlemanWunsch::NeedlemanWunsch(const std::string& fileOne, const std::string& fileTwo)
{
    FileParser fp1(fileOne);
    FileParser fp2(fileTwo);
    mSequenceA = fp1.GetSequence();
    mSequenceB = fp2.GetSequence();
    mHeaderA = fp1.GetHeader();
    mHeaderB = fp2.GetHeader();
}

void NeedlemanWunsch::RunAlgorithm()
{

    // Resize
    mScoreGrid.resize(mSequenceB.size() + 1);
    mDirGrid.resize(mSequenceB.size() + 1);
    for (int i = 0; i < mSequenceB.size() + 1; i++)
    {
        mScoreGrid[i].resize(mSequenceA.size() + 1);
        mDirGrid[i].resize(mSequenceA.size() + 1);
    }
    
    // Initialize data state
    mScoreGrid[0][0] = 0;
    mDirGrid[0][0] = ZERO;
    
    for (int i = 1; i < mSequenceB.size() + 1; i++)
    {
        mScoreGrid[i][0] = -i;
        mDirGrid[i][0] = UP;
    }
    
    for (int i = 1; i < mSequenceA.size() + 1; i++)
    {
        mScoreGrid[0][i] = -i;
        mDirGrid[0][i] = LEFT;
    }
    
    // Calculating scores
    int up = 0;
    int upLeft = 0;
    int left = 0;
    
    for (int i = 1; i < mSequenceB.size() + 1; i++)
    {
        for (int j = 1; j < mSequenceA.size() + 1; j++)
        {
            if(mSequenceA[j-1] == mSequenceB[i-1])
            {
                upLeft = mScoreGrid[i-1][j-1] + mMatchScore;
            }
            else
            {
                upLeft = mScoreGrid[i-1][j-1] + mMismatchScore;
            }
            
            left = mScoreGrid[i][j-1] + mGapScore;
            up = mScoreGrid[i-1][j] + mGapScore;
            
            if (upLeft >= left && upLeft >= up)
            {
                mScoreGrid[i][j] = upLeft;
                mDirGrid[i][j] = UP_LEFT;
            }
            else if (left >= up)
            {
                mScoreGrid[i][j] = left;
                mDirGrid[i][j] = LEFT;
            }
            else
            {
                mScoreGrid[i][j] = up;
                mDirGrid[i][j] = UP;
            }
        }
    }
    
    // Bottom right contains final score
    mScore = mScoreGrid[mSequenceB.size()][mSequenceA.size()];
    
    // Traceback
    int rowIndex = static_cast<int>(mSequenceB.size());
    int colIndex = static_cast<int>(mSequenceA.size());
    std::string tracebackA = "";
    std::string tracebackB = "";
    
    while (rowIndex != 0 || colIndex != 0)
    {
        if(mDirGrid[rowIndex][colIndex] == UP_LEFT)
        {
            tracebackA += mSequenceA[colIndex-1];
            tracebackB += mSequenceB[rowIndex-1];
            rowIndex--;
            colIndex--;
        }
        else if (mDirGrid[rowIndex][colIndex] == LEFT)
        {
            tracebackA += mSequenceA[colIndex-1];
            tracebackB += "_";
            colIndex--;
            
        }
        else if (mDirGrid[rowIndex][colIndex] == UP)
        {
            tracebackA += "_";
            tracebackB += mSequenceB[rowIndex-1];
            rowIndex--;
        }
    }
    
    mTraceBackA = tracebackA;
    mTraceBackB = tracebackB;
    
    
}

void NeedlemanWunsch::SaveFile(const std::string& fileName)
{
    std::ofstream out(fileName);
    out << "A: " << mHeaderA << std::endl;
    out << "B: " << mHeaderB << std::endl;
    out << "Score: " << mScore << std::endl << std::endl;
    
    std::reverse(mTraceBackA.begin(), mTraceBackA.end());
    std::reverse(mTraceBackB.begin(), mTraceBackB.end());
    
    //int loopSize = static_cast<int> (mTraceBackSeqA.size() / 70);

    for (int i = 0; i < mTraceBackA.size(); i++)
    {
        if (i % 70 == 0 && i != 0)
        {
            out << "\n" << mTraceBackB.substr(i - 70, 70) << std::endl << std::endl;
        }
        
        if (i % 70 == 0)
        {
            out << mTraceBackA.substr(i, 70) << std::endl;
        }
        
        if (mTraceBackA[i] == mTraceBackB[i])
        {
            out << "|";
        }
        else
        {
            out << " ";
        }
    }
    
    out << std::endl << mTraceBackB.substr(mTraceBackA.size() - mTraceBackA.size()%70, 70);
    
    
}
