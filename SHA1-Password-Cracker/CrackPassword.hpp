//
//  CrackPassword.hpp
//  password-mac
//
//  Created by Derek Cheng on 2/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#ifndef CrackPassword_hpp
#define CrackPassword_hpp

#include <stdio.h>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <list>
#include "DictionaryVal.h"
#include "PassStruct.h"

class CrackPassword
{
private:
    std::string dictionaryFileName;
    std::string passwordFileName;
    std::ifstream dictionaryFile;
    std::ifstream passwordFile;
    std::unordered_map<std::string, std::string> dictionaryMap;
    std::vector<PassStruct* > solvedVector;
    std::list<PassStruct* > unsolvedList;
    
    char Base36Conversion(int n);
    
public:
    CrackPassword(std::string dictionaryFileName, std::string passFileName);
    void LoadDictionary();
    void CheckPasswords();
    void BruteForce();
    void BruteForceHelper(int start, int end);
    void SaveFile();
    
};


#endif /* CrackPassword_hpp */
