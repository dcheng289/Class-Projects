// Main.cpp : Defines the entry point for the console application.
//

#include "RleTests.h"
#include "RleData.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include "RLEFile.h"

void Part1Tests()
{
	TestFixtureFactory::theInstance().runTests();
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		// Just the tests
		Part1Tests();
	}
	else
	{
        RleFile inFile;
        std::string fileName = argv[1];
        
        // If the file ends in .rl1, you want to decompress the file.
        // Any other extension on the file means you should compress the file.
        
        if (fileName.substr(fileName.length() - 4, fileName.length()) == ".rl1")
        {
            inFile.ExtractArchive(fileName);
        }
        else
        {
            inFile.CreateArchive(fileName);
        }
    }
	return 0;
}

