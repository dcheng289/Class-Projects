// Main.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include "Sha1.h"
#include "CrackPassword.hpp"



int main(int argc, char* argv[])
{
    // Simple SHA-1 Hash
    if (argc == 2) {
        unsigned char hash[20];
        char hex_str[41];
        sha1::calc(argv[1], strlen(argv[1]), hash);
        sha1::toHexString(hash, hex_str);
        std::cout<< hex_str << std::endl;
    }
    
    // Load dictionary and passwords, brute force crack
    if (argc == 3) {
        CrackPassword cp (argv[1], argv[2]);
        cp.LoadDictionary();
        cp.CheckPasswords();
        cp.BruteForce();
        cp.SaveFile();
    }
    return 0;
}

