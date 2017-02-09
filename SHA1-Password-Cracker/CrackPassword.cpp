//
//  CrackPassword.cpp
//  password-mac
//
//  Created by Derek Cheng on 2/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "CrackPassword.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "Timer.h"
#include "Sha1.h"
#include "PassStruct.h"

#include <tbb/parallel_invoke.h>


CrackPassword::CrackPassword(std::string dictionaryFileName, std::string passwordFileName)
{

    dictionaryFile = std::ifstream(dictionaryFileName);
    passwordFile = std::ifstream (passwordFileName);
    
    if(!dictionaryFile.is_open() && passwordFile.is_open())
        std::cout << "Dictionary file or password file cannot be found." << std::endl;
    else
    {
        this->dictionaryFileName = dictionaryFileName;
        this->passwordFileName = passwordFileName;
    }
}

void CrackPassword::LoadDictionary()
{

    dictionaryMap.rehash(100000);
    
    Timer timer;
    std::cout << "Loading dictionary..." << std::endl;
    timer.start();
    
    std::string dLine;
    
    while (std::getline(dictionaryFile, dLine))
    {
        DictionaryVal* nextVal = new DictionaryVal;
        nextVal->password = dLine;
        
        // Hash
        sha1::calc(dLine.c_str(), dLine.length(), nextVal->hash); // calc hash for dLine and store in hash
        sha1::toHexString(nextVal->hash, nextVal->hex_str); // store hash result into hex_str
        dictionaryMap.emplace(nextVal->hex_str, nextVal->password); // key-value is hashResult-password
    }
    dictionaryFile.close();
    
    // Timer
    double elapsed = timer.getElapsed();
    std::cout << "Finished loading dictionary. " << dictionaryMap.size() << " entries loaded in ";
    std::cout << elapsed << " seconds." << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;

    
}

void CrackPassword::CheckPasswords()
{
    std::string pLine;
    int lineNum = 0;
    solvedVector.reserve(5000);
    
    while (std::getline(passwordFile, pLine))
    {
        lineNum++;
        auto it = dictionaryMap.find(pLine);
        PassStruct* ps = new PassStruct;
        
        if (it != dictionaryMap.end())  // Solved
        {
            ps-> lineNum = lineNum;
            ps->hex_str = it->first;
            ps->plain_str = it->second;
        }
        else    // Unsolved
        {
            ps->lineNum = lineNum;
            ps->hex_str = pLine;
            unsolvedList.push_back(ps);   // Keep track of unsolved passwords
        }
        solvedVector.push_back(ps); // push all passwords in, the unsolved have null plain_str
    }
    
    std::cout << "Password file had " << lineNum << " passwords stored." << std:: endl;
    passwordFile.close();
    std::cout << "Solved " << solvedVector.size()-unsolvedList.size() << " passwords. ";
    std::cout << "Still have " << unsolvedList.size() << " to decrypt."<< std::endl;
}

void CrackPassword::BruteForce()
{
    std::cout << "Brute forcing remaining passwords..." << std::endl;
    
    Timer bTimer;
    bTimer.start();
    
    // Length 1 password
    int lengthOne[1] = {0};
    int i_max = pow(36,1);
    
    for (int i = 0; i < i_max; i++)
    {
        char charOne[2] = {'0','\0'};
        for (int j = 0; j < 1; j++)
        {
            charOne[j] = Base36Conversion(lengthOne[0]);
        }
        
        DictionaryVal* hashVal = new DictionaryVal;
        sha1::calc(charOne, 1, hashVal->hash);
        sha1::toHexString(hashVal->hash, hashVal->hex_str);
        
        for (auto it = unsolvedList.begin(); it != unsolvedList.end(); ++it)
        {
            if (hashVal->hex_str == (*it)->hex_str)
            {
                PassStruct* ps = new PassStruct;
                ps->lineNum = (*it)->lineNum;   // not necessary
                ps->hex_str = hashVal->hex_str;
                ps->plain_str = charOne;
                solvedVector[(*it)->lineNum - 1] = ps;
                std::cout << ps->lineNum << " -- " << ps->hex_str << " , " << ps->plain_str << std::endl;
            }
        }
        
        // Increment Counting Machine
        lengthOne[0]++;
    }
    
    // Length 2 password
    int lengthTwo[2] = {0, 0};
    i_max = pow(36,2);
    for (int i = 0; i < i_max; i++)
    {
        char charTwo[3] = {'0','0','\0'};
        for (int j = 0; j < 2; j++)
        {
            charTwo[j] = Base36Conversion(lengthTwo[j]);
        }
        
        DictionaryVal* hashVal = new DictionaryVal;
        sha1::calc(charTwo, 2, hashVal->hash);
        sha1::toHexString(hashVal->hash, hashVal->hex_str);

        for (auto it = unsolvedList.begin(); it != unsolvedList.end(); ++it)
        {
            if (hashVal->hex_str == (*it)->hex_str)
            {
                PassStruct* ps = new PassStruct;
                ps->lineNum = (*it)->lineNum;
                ps->hex_str = hashVal->hex_str;
                ps->plain_str = charTwo;
                solvedVector[(*it)->lineNum - 1] = ps;
                std::cout << ps->lineNum << " -- " << ps->hex_str << " , " << ps->plain_str << std::endl;
            }
        }
        
        // Increment Counting Machine
        lengthTwo[1]++;
        if (lengthTwo[1] == 36) // carry the one
        {
            lengthTwo[0]++;
            lengthTwo[1] = 0;
        }
        
    }
    
    // Length 3 password
    int lengthThree[3] = {0, 0, 0};
    i_max = pow(36,3);
    for (int i = 0; i < i_max; i++)
    {
        char charThree[4] = {'0','0','0','\0'};
        for (int j = 0; j < 3; j++)
        {
            charThree[j] = Base36Conversion(lengthThree[j]);
        }
        
        DictionaryVal* hashVal = new DictionaryVal;
        sha1::calc(charThree, 3, hashVal->hash);
        sha1::toHexString(hashVal->hash, hashVal->hex_str);

        
        for (auto it = unsolvedList.begin(); it != unsolvedList.end(); ++it)
        {
            if (hashVal->hex_str == (*it)->hex_str)
            {
                PassStruct* ps = new PassStruct;
                ps->lineNum = (*it)->lineNum;   // not necessary
                ps->hex_str = hashVal->hex_str;
                ps->plain_str = charThree;
                solvedVector[(*it)->lineNum - 1] = ps;
            }
        }
        
        // Increment Counting Machine
        lengthThree[2]++;
        if (lengthThree[2] == 36) // carry the one
        {
            lengthThree[1]++;
            lengthThree[2] = 0;
        }
        if (lengthThree[1] == 36) // carry the one
        {
            lengthThree[0]++;
            lengthThree[1] = 0;
        }
    }

    // Length 4 Password - Parallel Invoke
    tbb::parallel_invoke (
                          [this] { BruteForceHelper(0, 4); },
                          [this] { BruteForceHelper(4, 8); },
                          [this] { BruteForceHelper(8, 12); },
                          [this] { BruteForceHelper(12, 16); },
                          [this] { BruteForceHelper(16, 20); },
                          [this] { BruteForceHelper(20, 24); },
                          [this] { BruteForceHelper(24, 28); },
                          [this] { BruteForceHelper(28, 32); },
                          [this] { BruteForceHelper(32, 36); });
    
    double bElapsed = bTimer.getElapsed();
    
    // Count how many solved
    int bruteSolved = 0;
    for (auto it = unsolvedList.begin(); it != unsolvedList.end(); ++it)
    {
        if (solvedVector[(*it)->lineNum - 1]->plain_str != "??")
            bruteSolved++;
    }
    
    std::cout <<"Brute solved an additional " << bruteSolved;
    std::cout << " passwords in " << bElapsed << " seconds." << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
}

void CrackPassword::BruteForceHelper(int start, int end)
{

    int lengthFour[4] = {start, 0, 0, 0};
    int i_max = pow(36,3)*4;
 
    for (int i = 0; i < i_max; i++) // 0123 or 4567 etc
    {
        char charFour[5] = {'0','0','0','0','\0'};
        for (int j = 0; j < 4; j++)
        {
            charFour[j] = Base36Conversion(lengthFour[j]);
        }
        
        DictionaryVal* hashVal = new DictionaryVal;
        sha1::calc(charFour, 4, hashVal->hash);
        sha1::toHexString(hashVal->hash, hashVal->hex_str);
        
        for (auto it = unsolvedList.begin(); it != unsolvedList.end(); ++it)
        {
            if (hashVal->hex_str == (*it)->hex_str)
            {
                PassStruct* ps = new PassStruct;
                ps->lineNum = (*it)->lineNum;
                ps->hex_str = hashVal->hex_str;
                ps->plain_str = charFour;
                solvedVector[(*it)->lineNum - 1] = ps;
            }
        }
        
        // Increment Counting Machine
        lengthFour[3]++;
        if (lengthFour[3] == 36) // carry the one
        {
            lengthFour[2]++;
            lengthFour[3] = 0;
        }
        if (lengthFour[2] == 36) // carry the one
        {
            lengthFour[1]++;
            lengthFour[2] = 0;
        }
        if (lengthFour[1] == 36) // carry the one
        {
            lengthFour[0]++;
            lengthFour[1] = 0;
        }
    }
    
}

void CrackPassword::SaveFile()
{
    std::ofstream out("pass_solved.txt");
    for(int i = 0; i < solvedVector.size(); i++)
    {
        out << solvedVector[i]->hex_str << ",";
        out << solvedVector[i]->plain_str << std::endl;
    }
    out.close();
}

char CrackPassword::Base36Conversion(int n)
{
    if (n < 26)
        return static_cast<char>(n) + 'a';
    else
        return static_cast<char>(n) + '0' - 26;
}
