%module testswig 
%{
#include "testswig.h"
%}
// Now list ISO C/C++ declarations
uint8_t* getBuffer(const std::string& str);

#define IN
#define OUT

%apply unsigned int { uint32_t };

%typemap(argout) uint8_t* OUTPUT {
    
}

%apply uint8_t * OUTPUT {uint8_t* buffer};
extern "C" uint32_t testOut(IN char* testStr, OUT uint8_t* buffer);

extern "C" int testSimpleOut(IN char* testStr, OUT uint8_t* buffer);