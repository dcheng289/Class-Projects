//
//  FileParser.hpp
//  dnawx-mac
//
//  Created by Derek Cheng on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#ifndef FileParser_hpp
#define FileParser_hpp

#include <string>
#include <stdio.h>
#pragma once

class FileParser
{
    
public:
    FileParser(const std::string& path);
    std::string GetHeader() const;
    std::string GetSequence() const;
    
private:
    std::string mHeader = "";
    std::string mSequence = "";
    
};


#endif /* FileParser_hpp */
