#include "RLEFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "FileSystem.h"

void RleFile::CreateArchive(const std::string& source)
{

    std::ifstream::pos_type size;
    char* memblock;
    
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char [static_cast<unsigned int>(size)];
        file.seekg (0, std::ios::beg); // Seek back to start of file
        file.read (memblock, size);
        file.close();
        // File data has now been loaded into memblock array
        
        // Compress
        mData.Compress(memblock, static_cast<unsigned>(size));
        double percentCompress = static_cast<double>((static_cast<int>(size) - mData.mSize)) * 100 / static_cast<int>(size);
        std::cout << "File was successfully compressed by " << percentCompress << "%." << std::endl;

        // Prepare RLE file
        std::string fileName = source + ".rl1";
     
        mHeader.sig[0] = 'R';
        mHeader.sig[1] = 'L';
        mHeader.sig[2] = 'E';
        mHeader.sig[3] = '\x01';
        mHeader.fileSize = static_cast<int>(size);
        mHeader.fileNameLength = static_cast<char>(source.length());
        mHeader.fileName = source;

        // Open the file for output, in binary mode, and overwrite an existing file
        std::ofstream arc(fileName, std::ios::out|std::ios::binary|std::ios::trunc);
        if (arc.is_open())
        {
            // Writing data
            arc.write(mHeader.sig, 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.fileSize)), 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.fileNameLength)), 1);
            arc.write(mHeader.fileName.c_str(), mHeader.fileNameLength);
            arc.write(mData.mData, mData.mSize);
        }
        
        // Cleaning up
        delete[] memblock;
    }
    

}

void RleFile::ExtractArchive(const std::string& source)
{
    std::ifstream::pos_type size;
    char* memblock;
    
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char [static_cast<unsigned int>(size)];
        file.seekg (0, std::ios::beg); // Seek back to start of file
        file.read (memblock, size);
        file.close();
        // File data has now been loaded into memblock array
        
       // Check file signature
        bool isRLE1 = false;
        if (memblock[0] == 'R' && memblock[1] == 'L' && memblock[2] == 'E' && memblock[3] == '\x01')
            isRLE1 = true;
        
        if (!isRLE1) {
            std::cout << "Error! Cannot extract a non-RLE1 file." << std::endl;
            exit(1);
        }
        else {
            
            mHeader.fileSize = *(reinterpret_cast<int*>(&memblock[4]));
            mHeader.fileNameLength = *(reinterpret_cast<int*>(&memblock[8]));
            
            // Get the file name
            int fileNameIndex = 8;
            for (int i = fileNameIndex + 1; i < fileNameIndex + mHeader.fileNameLength + 1; i++) {
                mHeader.fileName += memblock[i];
            }
            std::string fileName = mHeader.fileName;
            
            // Decompress
            int dataIndex = fileNameIndex + static_cast<int>(mHeader.fileNameLength) + 1;
            mData.Decompress(&(memblock[dataIndex]), static_cast<int>(size) - 8 - static_cast<int>(mHeader.fileNameLength) - 1, mHeader.fileSize);
            
            // Open the file for output, in binary mode, and overwrite an existing file
            std::ofstream arc(fileName, std::ios::out|std::ios::binary|std::ios::trunc);
            if (arc.is_open()) {
                arc.write(mData.mData, mData.mSize);
            }

            std::cout << "File was successfully decompressed." << std::endl;
        }

        // Cleaning up
        delete[] memblock;
    }
}
