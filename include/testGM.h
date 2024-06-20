#include <stdint.h>
typedef uint8_t u8;
typedef uint32_t u32;

#define IN 
#define OUT
#define INOUT


#define MAX_ALIAS_LEN 64
struct key_st
{
    char pszAlias[MAX_ALIAS_LEN];
    u32 u32Version;
};

typedef key_st Key;
// TODO 调用流程确定   要协商密钥吗？ 协商一次密钥后续所有请求都通过这个通道吗？   通过 sendDataProtect 和 recvDataProtect 两个接口来收发数据吗？
// 服务器启动与密码服务平台协商密钥，然后通过 sendDataProtect 和 recvDataProtect 两个接口来收发数据？  服务关闭， release 会话？
// 这个通道能保持多久?
// 还是每次发送请求都要协商？
// 还是通过 http 请求
u32 InitReqA(IN const char* pszUserId, IN const char* pszInitReqAData, OUT char* pszInitRespBData);
u32 AuthReqA(IN const char* pszUserId, IN const char* pszInitAuthAData, OUT char* pszAuthRespBData);

// TODO 文档中有问题 算法表示字符串应该是 SM2， json中的 key 应该是 base64SecretKey
u32 ImportSymmKey(INOUT Key* pKey,IN u32 u32Period, IN const char* pszSymmAlg, IN u32 u32Length, IN const u8* pu8SecretKey);
u32 DeleteKey(INOUT Key* pKey);
u32 EnableKey(INOUT Key* pKey);
u32 DisableKey(INOUT Key* pKey);

u32 GetRandom(u32 u32Length, OUT u8* pu8Random);
u32 HashEx(IN const char* pszAlg, IN const u8* pu8Data, u32 u32DataLen, OUT u8** ppu8Hash, OUT u32* pu32HashLen);
u32 HAMCEx(IN const Key* pKey, IN const char* pszHmacAlg, IN const u8* pu8Data, u32 u32DataLen, OUT u8** ppu8Hmac, OUT u32* pu32HmacLen);

// TODO 密码服务平台的接口有问题  IV 应该经过 base64编码， 文档中的没有体现
u32 SM4EncryptEx(IN const Key* pKey, IN const char* pszPaddingMode, IN const u8* pu8Data, u32 u32DataLen, IN const u8* pu8Iv, u32 u32IvLen, OUT u8** ppu8CipherData, OUT u32* pu32CipherDataLen);
u32 SM4DecryptEx(IN const Key* pKey, IN const char* pszPaddingMode, IN const u8* pu8CipherData, u32 u32CipherDataLen, IN const u8* pu8Iv, u32 u32IvLen, OUT u8** ppu8Data, OUT u32* pu32DataLen);

u32 SignEx(IN const Key* pKey, IN const char* signAlg, IN const u8* pu8Data, u32 u32DataLen, OUT u8** ppu8Sign, OUT u32* pu32SignLen);
u32 Verify(IN const Key* pKey, IN const char* signAlg, IN const u8* pu8Data, u32 u32DataLen, IN const u8* pu8Sign, u32 u32SignLen);

u32 AsymEncryptEx(IN const Key* pKey, IN const char* encAlg, IN const u8* pu8Data, u32 u32DataLen, OUT u8** ppu8CipherData, OUT u32* pu32CipherDataLen);
u32 AsymDecryptEx(IN const Key* pKey, IN const char* encAlg, IN const u8* pu8CipherData, u32 u32CipherDataLen, OUT u8** ppu8Data, OUT u32* pu32DataLen);

