#include <iostream>
#include <string>

std::string bytesToHexString(const uint8_t* buf, int len, char sep)
{
    std::string hexString;
    static const char hexdig[] = "0123456789ABCDEF";
    int hasSep = sep == '\0' ? 0 : 1;

    if(buf == nullptr || len <= 0)
    {
        return hexString;
    }

    hexString.reserve(len * (2 + hasSep));

    int i = 0;
    const uint8_t* p = buf;
    for (; i < len; i++, p++) {
        hexString += hexdig[(*p >> 4) & 0xf];
        hexString += hexdig[*p & 0xf];
        if(hasSep){
            hexString += sep;
        }
    }

    return hexString;
}

// hexCharToByte converts a hex character to its integer value.
static uint8_t hexCharToByte(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
		return ch - '0';
	}
    else if (ch >= 'A' && ch <= 'F')
    {
		return ch - 'A' + 10;
	}
    else if (ch >= 'a' && ch <= 'f')
    {
		return ch - 'a' + 10;
	}
    else
    {
		return 0;
	}
}

uint8_t* hexStringToBytes(const std::string& hexString, int* outLen, char sep)
{
	uint8_t* buf = nullptr;
	int len = 0;
	int hasSep = sep == '\0' ? 0 : 1;

    if (hexString.empty())
    {
		return buf;
	}

	len = hexString.length() / (2 + hasSep);
	buf = (uint8_t*)malloc(len);
    if (buf == nullptr)
    {
		return buf;
	}

	int i = 0;
	int j = 0;
    for (; i < len; i++, j += 2 + hasSep) {
		buf[i] = (hexCharToByte(hexString[j]) << 4) | hexCharToByte(hexString[j + 1]);
	}

    if (outLen != nullptr)
    {
		*outLen = len;
	}

	return buf;
}

