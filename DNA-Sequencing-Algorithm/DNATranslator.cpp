//
//  DNATranslator.cpp
//  dnawx-mac
//
//  Created by Derek Cheng on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "DNATranslator.hpp"

DNATranslator::DNATranslator(const std::string& sequence)
{
    mSequence = sequence;
    int state = 0;
    int nucleo = 0;
    
    for (int i = 0; i < mSequence.size(); i++)
    {
        if(mSequence[i] == 'T')
        {
            nucleo = 0;
            //state = mStateMachine[state][nucleo];
            //IncrementAmino(state);
        }
        else if (mSequence[i] == 'C')
        {
            nucleo = 1;
            //state = mStateMachine[state][nucleo];
            //IncrementAmino(state);
        }
        else if (mSequence[i] == 'A')
        {
            nucleo = 2;
            //state = mStateMachine[state][nucleo];
            //IncrementAmino(state);
        }
        else if (mSequence[i] == 'G')
        {
            nucleo = 3;
            //state = mStateMachine[state][nucleo];
            //IncrementAmino(state);
        }
        
        state = mStateMachine[state][nucleo];
        IncrementAmino(state);
        
        
    }
}

std::map<char, int> DNATranslator::GetAminoCounter() const
{
    return mAminoCounter;
}

const int DNATranslator::GetCount() const
{
    return mCount;
}


void DNATranslator::IncrementAmino(int& state)
{
    if(state >= 'A' && state <= 'Z')
    {
        mAminoCounter[state]++;
        mCount++;
        state = 3;
    }
}



// Update later
const int DNATranslator::GetMaxAminoAcidCount() const
{
    auto max = std::max_element(mAminoCounter.begin(), mAminoCounter.end(), [](const std::pair<char, int>& p1, const std::pair<char, int>& p2) { return p1.second < p2.second;});
    return max->second;
}

/*
 1) GetMaxAminoAcidCount in this class
 2) SetData and DrawBar in DrawPanel class
 3) TAS2R17 Homo Sapiens --> Tyrosine (5, should be 8) and Threonine (26, should be 23) counted wrong
*/
