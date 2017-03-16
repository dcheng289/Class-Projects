//
//  DNATranslator.hpp
//  dnawx-mac
//
//  Created by Derek Cheng on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#ifndef DNATranslator_hpp
#define DNATranslator_hpp

#include <stdio.h>
#include <stdio.h>
#include <string>
#include <map>

#endif /* DNATranslator_hpp */


class DNATranslator
{
public:
    DNATranslator (const std::string& sequence);
    std::map<char, int> GetAminoCounter() const;
    const int GetCount() const;
    
    // Update later
    const int GetMaxAminoAcidCount() const;
    void IncrementAmino(int& state);
    
private:
    std::string mSequence;
    int mCount = 0;

    
    std::map<char, int> mAminoCounter =
    {
        {'F', 0}, {'L', 0}, {'I', 0}, {'M', 0}, {'V', 0},
        {'S', 0}, {'P', 0}, {'T', 0}, {'A', 0}, {'Y', 0},
        {'H', 0}, {'Q', 0}, {'N', 0}, {'K', 0}, {'D', 0},
        {'E', 0}, {'C', 0}, {'W', 0}, {'R', 0}, {'G', 0}
    };
    
    const int mStateMachine[24][4] =
    {
        {0, 0, 1, 0},           // State 0
        {2, 0, 1, 0},           // State 1
        {0, 0, 1, 'M'},         //
        {4, 9, 14, 19},         // Start building
        {5, 6, 7, 8},           // Start of T
        {'F', 'F', 'L', 'L'},
        {'S', 'S', 'S', 'S'},
        {'Y', 'T', 0, 0},
        {'C', 'C', 0, 'W'},
        {10, 11, 12, 13},       // Start of C
        {'L', 'L', 'L', 'L'},
        {'P', 'P', 'P', 'P'},
        {'H', 'H', 'Q', 'Q'},
        {'R', 'R', 'R', 'R'},
        {15, 16, 17, 18},       // Start of A
        {'I', 'I', 'I', 'M'},
        {'T', 'T', 'T', 'T'},
        {'N', 'N', 'K', 'K'},
        {'S', 'S', 'R', 'R'},
        {20, 21, 22, 23},       // Start of G
        {'V', 'V', 'V', 'V'},
        {'A', 'A', 'A', 'A'},
        {'D', 'D', 'E', 'E'},
        {'G', 'G', 'G', 'G'},
    };
    
};



