//
//  PassStruct.h
//  password-mac
//
//  Created by Derek Cheng on 2/7/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#ifndef PassStruct_h
#define PassStruct_h

#include <stdio.h>
#include <iostream>
#include <string>

struct PassStruct
{
    int lineNum;
    std::string hex_str = "";
    std::string plain_str = "??";
};

#endif /* PassStruct_h */
