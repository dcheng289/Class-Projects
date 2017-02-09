#include "RleData.h"
#include <iostream>

void RleData::Compress(const char* input, size_t inSize)
{
    // Delete mData, reset mSize to zero, and dynamically allocate new mData
    mData = nullptr;
    mSize = 0;
    mData = new char[2*inSize];
    
    int runSize = 0x00;
    char runType = '\0';
    int negativeIndex = -1;

    for (int i = 0; i < inSize + 1; i++) {
        
        if(runType == '\0') {   // for first char in input
            runType = input[i];
        }
        
        if(input[i] == input[i+1]) { // Current and next are the same
            runSize++;
            
            if(input[i] == runType) {   // Add to existing positive Run

                if(runSize == MaxRunSize()) {   // Case of max positive run size reached
                    
                    // Writing to mData
                    mData[mSize] = runSize;
                    mSize++;
                    mData[mSize] = runType;
                    mSize++;
                
                    // Reset size and set runType to current i
                    runType = input[i]; // new runType is the current? or next one?
                    runSize = 0x00;
                }
            }
            else {  // End negative run with new positive run
                
                // Runs of size 1 are assigned -1
                int negativeRunSize = -(runSize - 1);
                
                // Case of runSize == 1 positive run
                if (negativeRunSize == -1)
                    negativeRunSize = 1;
                
                
                mData[mSize] = negativeRunSize; // Write negative run size
                mSize++;
                for (int j = negativeIndex; j < i; j++) { // Write negative run with prior terms
                    mData[mSize] = input[j];
                    mSize++;
                }
                
                // Set up the next run
                negativeIndex = -1;
                runType = input[i+1];
                runSize = 0x01; // actually at 0x01 since we're at 2 consecutive for the new positive
            }

            
        }
        
        else {  // Current and next are different
                runSize++;
            
            if( runSize != 0x01 && negativeIndex == -1) { // End positive run

                mData[mSize] = runSize;
                mSize++;
                mData[mSize] = runType;
                mSize++;

                // Set up the next run
                runType = input[i+1];
                runSize = 0x00; // fFrst of a new negative or positive run
            }
            else {  // Add to negative run
                runType = input[i]; // update type to current
                
                if(negativeIndex == -1) {
                    negativeIndex = i;    // first of negative index was last loop
                }
                
                // Case of reaching terminating char, so write negative run to data
                if(i == inSize -1) {

                    mData[mSize] = -runSize;     // write negative run size
                    mSize++;
                    
                    for (int j = negativeIndex; j < i+1; j++) { // write negative run
                        mData[mSize] = input[j];
                        mSize++;
                    }
                }
                
                // Case of reaching max negative run size
                if(runSize == MaxRunSize()) {

                    mData[mSize] = -runSize;
                    mSize++;
                    for (int j = negativeIndex; j < i+1; j++) { // write negative run
                        mData[mSize] = input[j];
                        mSize++;
                    }
                    
                    negativeIndex = -1;
                    runType = input[i+1];   // new runType is the next index's
                    runSize = 0x00;
                }
            }
        }
    }
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
    
    mData = nullptr;
    mSize = 0;
    mData = new char[outSize];
    
    // First element
    int runSize; // negative or positive run
    char runType;
    
    for (int i = 0; i < inSize; i ++) {
        
        // First write the runSize (negative or positive run) and iterate to char after
        runSize = input[i];
        i++;
        
        if (runSize > 0) {  // Positive Run
            runType = input[i];
            
            // Write the positive run
            for (int j = 0; j < runSize; j++) {
                mData[mSize] = runType;
                mSize++;
            }
        }
        else {              // Negative run
            
            // Write the negative run
            for (int j = 0; j < -runSize; j++) {
                runType = input[i+j];
                mData[mSize] = runType;
                mSize++;
            }
            i += -runSize - 1;  // Increment i by the number of times j was looped

        }
    }

}
