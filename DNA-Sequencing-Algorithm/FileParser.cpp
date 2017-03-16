//
//  FileParser.cpp
//  dnawx-mac
//
//  Created by Derek Cheng on 3/6/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "FileParser.hpp"
#include "Exceptions.h"
#include <iostream>
#include <fstream>

FileParser::FileParser(const std::string& path)
{
    std::ifstream::pos_type size;
    std::ifstream file(path);
    
    if(file.is_open())
    {
        size = file.tellg(); // save file size
        
        mSequence.reserve(size);
        
        char buf;
        file.get(buf);
        
        if ( buf != '>' && buf != '\n')
        {
            throw FileLoadExcept();
        }
        
        // Parse the header until the newline is hit
        while (file.get(buf) && buf != '\n')
        {
            mHeader += buf;
            //std::cout << buf;
        }
        
        
        while (file.get(buf))
        {
            if(buf == 'T' || buf == 'C' || buf == 'A' || buf == 'G')
            {
                mSequence += buf;
                //std::cout << buf;
            }
            else if (buf == '\n' || buf == '\r' || buf == '\xff')
            {
                // Do nothing
            }
            else // Bad char
            {
                throw FileLoadExcept();
            }
        }
        
        
        //std::cout << mSequence;
        
    }
    
}

std::string FileParser::GetHeader() const
{
    return mHeader;
}

std::string FileParser::GetSequence() const
{
    return mSequence;
}
