#ifndef _HEXUTIL_H_
#define _HEXUTIL_H_

#include <string>

std::string bytesToHexString(const uint8_t* buf, int len, char sep);
uint8_t* hexStringToBytes(const std::string& hexString, int* outLen, char sep);

#endif