#include <iostream>

#include "../include/rice.hpp"

namespace rice {
RiceDecoder::RiceDecoder(data::RiceEncodedData& encodedData)
    : input(encodedData.encodedData)
    , dataCount(encodedData.dataCount)
    , optimumRiceParam(encodedData.optimumRiceParam)
{
}

inline void RiceDecoder::generateEncodedBits()
{
    bitInput.reserve(input.size() * 32);
    for (uint64_t i = 0; i < input.size(); i++) {
        for (uint64_t j = 0; j < 32; j++) {
            bitInput.push_back((bool)(input[i] >> j & 0x1));
        }
    }
}

inline void RiceDecoder::generateDecodedUnsignedInts()
{
    uint32_t count = 0; 
    uint32_t temp = 0;
    uint32_t i = 0;
    uint32_t bitReadCounter = 0;
    unsignedOutput.reserve(dataCount);
    while (count < dataCount) {
        // Count 1s until a zero is encountered
        temp = 0;
        while (bitInput[bitReadCounter] == 1) {
            bitReadCounter++;
            temp++;
        }
        unsignedOutput.push_back(temp << optimumRiceParam);
        // Read the last 'optimumRiceParam' number of bits and add them to output
        for (i = 1; i < (optimumRiceParam + 1); i++) {
            unsignedOutput[count] = unsignedOutput[count] | ((long)bitInput[bitReadCounter] << (optimumRiceParam - i));
            bitReadCounter++;
        }
        count++;
    }
}

inline void RiceDecoder::convertUnsignedToSigned()
{
    output.reserve(dataCount);
    for (uint64_t x : unsignedOutput) {
        output.push_back(
            (int32_t)(((x & 0x01) == 0x01) ? -(int64_t)((x + 1) >> 1) : (x >> 1)));
    }
}

data::RiceDecodedData RiceDecoder::process()
{
    generateEncodedBits();
    generateDecodedUnsignedInts();
    convertUnsignedToSigned();
    return *(new data::RiceDecodedData(output));
}
}
