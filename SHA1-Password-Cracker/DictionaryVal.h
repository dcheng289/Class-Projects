//
//  DictionaryVal.h
//  password-mac
//
//  Created by Derek Cheng on 2/7/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#ifndef DictionaryVal_h
#define DictionaryVal_h
//
//  dictionaryVal.hpp
//  password-mac
//
//  Created by Derek Cheng on 2/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#ifndef dictionaryVal_hpp
#define dictionaryVal_hpp

#include <stdio.h>
#include <iostream>
#include <string>

struct DictionaryVal
{
    std::string password = "";
    unsigned char hash[20] = "";    
    char hex_str[41] = "";
};


#endif /* dictionaryVal_hpp */


#endif /* DictionaryVal_h */
