#ifndef _TESTSWIG_H_
#define _TESTSWIG_H_

#include <vector>
#include <string>
#include <unordered_map>

uint8_t* getBuffer(const std::string& str);

#define IN
#define OUT
extern "C" uint32_t testOut(IN char* testStr, OUT uint8_t* buffer);

extern "C" int testSimpleOut(IN char* testStr, OUT uint8_t* buffer);

#endif
