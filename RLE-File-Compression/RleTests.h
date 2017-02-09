#pragma once
#include "MiniCppUnit.hpp"
#include "RleData.h"
#include <iostream>
#include <iomanip>

// Helper functions
bool buffersAreSame(const char* expected, const char* actual, size_t size);
void runCompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);
void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);

// IMPORTANT NOTE --
// The tests all use "sizeof" to find the size of the input
// buffer. However, remember that this only works because the
// character arrays are statically allocated.
// If you just have a pointer to a C-style string, "sizeof"
// will return the size of the pointer type (either 4 or 8).
class CompressionTests : public TestFixture<CompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(CompressionTests, "Testing Compression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive runs test");
        // Added compression test cases below
        TEST_CASE_DESCRIBE(testLongPositiveRuns, "Long positive runs test");
        TEST_CASE_DESCRIBE(testLongLongPositiveRuns, "Really long positive runs test");
        TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic negative runs test");
        TEST_CASE_DESCRIBE(testLongNegativeRuns, "Long negative runs test");
        TEST_CASE_DESCRIBE(testLongLongNegativeRuns, "Really long negative runs test");
        TEST_CASE_DESCRIBE(testBasicAlternatingRuns1, "Basic alternating runs test1");
        TEST_CASE_DESCRIBE(testBasicAlternatingRuns2, "Basic alternating runs test2");
        TEST_CASE_DESCRIBE(testLongLongAlternatingRuns, "Really long alternating runs test");
        TEST_CASE_DESCRIBE(testAlternatingRuns1, "Alternating runs test1");
        TEST_CASE_DESCRIBE(testAlternatingRuns2, "Alternating runs test2");
        TEST_CASE_DESCRIBE(testAlternatingRuns3, "Alternating runs test3");
        TEST_CASE_DESCRIBE(testAlternatingRuns4, "Alternating runs test4");
        TEST_CASE_DESCRIBE(testPositiveMaxRuns1, "Max positive size runs test1");
        TEST_CASE_DESCRIBE(testPositiveMaxRuns2, "Max positive size runs test2");
        TEST_CASE_DESCRIBE(testPositiveMaxRuns3, "Max positive size runs test3");
        TEST_CASE_DESCRIBE(testPositiveMaxRuns4, "Max positive size runs test4");
        TEST_CASE_DESCRIBE(testPositiveMaxRuns5, "Max positive size runs test5");
        TEST_CASE_DESCRIBE(testPositiveMaxRuns6, "Max positive size runs test6");
        TEST_CASE_DESCRIBE(testPositiveMaxRuns7, "Max positive size runs test7");
         
        TEST_CASE_DESCRIBE(testNumbersRuns1, "Numbers runs test1");
        TEST_CASE_DESCRIBE(testNumbersRuns2, "Numbers runs test2");
        TEST_CASE_DESCRIBE(testNumbersRuns3, "Numbers runs test3");
        TEST_CASE_DESCRIBE(testNullRuns1, "Null char runs test");
    }
    
    void testBasicPositiveRuns()
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
			"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
    
    void testLongPositiveRuns()     // 50 chars each
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "cccccccccccccccccccccccccccccccccccccccccccccccccc"
            "dddddddddddddddddddddddddddddddddddddddddddddddddd";
        
        char expected[] = "\x32" "a" "\x32" "b" "\x32" "c" "\x32" "d";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
    
    void testLongLongPositiveRuns() // 300 chars, too long, where max size is -127 to 127
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        
        char expected[] = "\x7F" "a" "\x7F" "a" "\x2E" "a";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    
    void testBasicNegativeRuns()
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz";
        char expected[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongNegativeRuns() // 50 chars plus bcde
    {

        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcde";
        char expected[] = "\x32" "a" "\xFC" "b" "c" "d" "e";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongLongNegativeRuns() // 260 chars --> 127 (52*2 + 23) , 127, 6
    {
        char test[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        
        char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
            "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
            "abcdefghijklmnopqrstuvw"
                "\x81" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrst"
                "\xFA" "uvwxyz";
 
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
    
    void testBasicAlternatingRuns1() // 23 mixed
    {
        char test[] = "aaabceddddddabcddddabcd";
        
        char expected[] = "\x03" "a" "\xFD" "bce" "\x06" "d" "\xFD" "abc" "\x04" "d" "\xFC" "abcd";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
    
    void testBasicAlternatingRuns2()
    {
        char test[] = "aaaabccccdeeeeabcdefgaaaabccccdeeeeabcdefg";
        char expected[] = "\x04" "a" "\x01" "b" "\x04" "c" "\x01" "d" "\x04" "e" "\xf9" "abcdefg"
        "\x04" "a" "\x01" "b" "\x04" "c" "\x01" "d" "\x04" "e"
        "\xf9" "abcdefg";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongLongAlternatingRuns()  // 300 a's, 260 negatives, 23 mixed
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "bacdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "aaabceddddddabcddddabcd";
        
        char expected[] = "\x7F" "a" "\x7F" "a" "\x2E" "a"  // 300 a's
        "\x81" "bacdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"   // 260 negatives
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvw"
        "\x81" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrst"
        "\xFA" "uvwxyz"
        "\x03" "a" "\xFD" "bce" "\x06" "d" "\xFD" "abc" "\x04" "d" "\xFC" "abcd";   // 23 mixed
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
   
    void testAlternatingRuns1()
    {
        char test[] = "aaabbbcccdefghiaaab";
        char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\xFA" "defghi" "\x03" "a" "\xFF" "b";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testAlternatingRuns2()
    {
        char test[] = "aaaaaaaaaaaaab";
        char expected[] = "\x0D" "a" "\xFF" "b";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testAlternatingRuns3()
    {
        char test[] = "baaaaaaaaaaaaa";
        char expected[] = "\x01" "b" "\x0D" "a";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testAlternatingRuns4()
    {
        char test[] = "baaaaaaaaaaaaab";
        char expected[] = "\x01" "b" "\x0D" "a" "\xFF" "b";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testPositiveMaxRuns1()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x7F" "a" "\x11" "a";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testPositiveMaxRuns2()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaabbc";
        char expected[] = "\x7F" "a" "\x02" "b" "\xFF" "c";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testPositiveMaxRuns3()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaabbb";
        char expected[] = "\x7F" "a" "\x03" "b";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testPositiveMaxRuns4()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x7F" "a" "\xFF" "a";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testPositiveMaxRuns5()
    {
        char test[] = "ababababababababababababababababababababababababababababababababab"
        "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdc";
        
        char expected[] = "\x81" "ababababababababababababababababababababababababababababababababab"
        "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdc";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testPositiveMaxRuns6()
    {
        char test[] = "ababababababababababababababababababababababababababababababababab"
        "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd";
        
        char expected[] = "\x81" "ababababababababababababababababababababababababababababababababab"
        "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdc" "\xFF" "d";
        
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testPositiveMaxRuns7()
    {
        char test[] = "ababababababababababababababababababababababababababababababababab"
        "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcxyz";
        
        char expected[] = "\x81" "ababababababababababababababababababababababababababababababababab"
        "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdc" "\xFD" "xyz";
        
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }

    void testNumbersRuns1()
    {
        char test[] = "0011001100";
        
        char expected[] = "\x02" "0" "\x02" "1" "\x02" "0" "\x02" "1" "\x02" "0";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testNumbersRuns2()
    {
        char test[] = "01234567890";
        
        char expected[] = "\xF5" "01234567890";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testNumbersRuns3()
    {
        char test[] = "0111010010";
        
        char expected[] = "\x01" "0" "\x03" "1" "\xFE" "01" "\x02" "0" "\xFE" "10";

        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }

    void testNullRuns1()
    {
        char test[] = "\x00" "\x00" "\x00" "\x01" "\x01";
        char expected [] = "\x03" "\x00" "\x02" "\x01";
        
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    
    
};

class DecompressionTests : public TestFixture<DecompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(DecompressionTests, "Testing Decompression...")
	{
        
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive run test");
        // Added decompression test cases below
        TEST_CASE_DESCRIBE(testLongPositiveRuns, "Long positive runs test");
        TEST_CASE_DESCRIBE(testLongLongPositiveRuns, "Really long positive runs test");
        TEST_CASE_DESCRIBE(testLongNegativeRuns, "Long negative runs test");
        TEST_CASE_DESCRIBE(testLongLongNegativeRuns, "Really long negative runs test");
        TEST_CASE_DESCRIBE(testBasicAlternatingRuns, "Basic alternating runs test");
        TEST_CASE_DESCRIBE(testLongLongAlternatingRuns, "Really long alternating runs test");

      
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
		runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
    

    
    void testLongPositiveRuns()     // 50 chars each
    {
        char test[] = "\x32" "a" "\x32" "b" "\x32" "c" "\x32" "d";
        
        char expected[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
        "cccccccccccccccccccccccccccccccccccccccccccccccccc"
        "dddddddddddddddddddddddddddddddddddddddddddddddddd";
        
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
    
    void testLongLongPositiveRuns() // 300 chars, too long, where max size is -127 to 127
    {
        char test[] = "\x7F" "a" "\x7F" "a" "\x2E" "a";
        
        char expected[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongNegativeRuns() // 50 chars plus bcde
    {
        char test[] = "\x32" "a" "\xFC" "b" "c" "d" "e";

        char expected[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabcde";

        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongLongNegativeRuns() // 260 chars --> 127 (52*2 + 23) , 127, 6
    {
        char test[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvw"
        "\x81" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrst"
        "\xFA" "uvwxyz";
        
        char expected[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
    
    void testBasicAlternatingRuns() // 23 mixed
    {
        char test[] = "\x03" "a" "\xFD" "bce" "\x06" "d" "\xFD" "abc" "\x04" "d" "\xFC" "abcd";
        
        char expected[] = "aaabceddddddabcddddabcd";

        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
    
    void testLongLongAlternatingRuns()  // 300 a's, 260 negatives, 23 mixed
    {
        char test[] = "\x7F" "a" "\x7F" "a" "\x2E" "a"  // 300 a's
        "\x81" "bacdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"   // 260 negatives
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvw"
        "\x81" "xyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrst"
        "\xFA" "uvwxyz"
        "\x03" "a" "\xFD" "bce" "\x06" "d" "\xFD" "abc" "\x04" "d" "\xFC" "abcd";   // 23 mixed
        
        char expected[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "bacdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
        "aaabceddddddabcddddabcd";
        
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
        
    }
};

REGISTER_FIXTURE(CompressionTests);
REGISTER_FIXTURE(DecompressionTests);

bool buffersAreSame(const char* expected, const char* actual, size_t size)
{
	if (actual == nullptr)
	{
		std::cerr << std::endl << "Actual data is null." << std::endl;
		return false;
	}

	bool retVal = true;

	for (size_t i = 0; i < size; i++)
	{
		if (expected[i] != actual[i])
		{
			retVal = false;
			std::cerr << std::endl
				<< "Expected 0x" << std::hex << static_cast<unsigned>(expected[i])
				<< " at index " << std::dec << i << " but saw 0x"
				<< std::hex << static_cast<unsigned>(actual[i]) << std::endl;
			break;
		}
	}

	return retVal;
}

void runCompressionTest(char* test, size_t testSize, 
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Compress(test, testSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}

void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Decompress(test, testSize, expectedSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}
