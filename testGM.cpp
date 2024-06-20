#include <iostream>

#include <cstdint>
#include <memory.h>


#include <nlohmann/json.hpp>
#include <httplib.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <hexUtil.h>

#define MAX_ALIAS_LEN 64
struct Key 
{
    char pszAlias[MAX_ALIAS_LEN];
    uint32_t u32Version;
};

std::string ip = "127.0.0.1";
std::string port = "8080";

/*
 TODO 
 1. 用 openssl 接口实现 base64 编码解码  done
 2. 用 openssl 接口实现下面定义的接口（加密解密，签名验签） 然后替换 Tongsuo 中的国密实现，调试通过
 3. 等待密码服务平台接口实现，调试通过后，再替换进入 Tongsuo, 调试通过
*/

#define IF_ERR_BREAK if(ret != 0) {break;}

// https://openssl-programing.readthedocs.io/en/latest/12.html
uint32_t base64Encode(const uint8_t* pu8Data, uint32_t u32DataLen, std::string& strBase64)
{
    const auto base64StrLen = 4 * ((u32DataLen + 2) / 3);

    auto output = std::make_unique<char[]>(base64StrLen + 1);

    const auto resultLen = EVP_EncodeBlock(reinterpret_cast<uint8_t*>(output.get()), pu8Data, u32DataLen);
    if(resultLen != base64StrLen)
    {
        return 1;
    }

    strBase64 = std::string(output.get(), resultLen);
    return 0;
}


uint32_t base64DecodeEx(const std::string& strBase64, uint8_t** ppu8Data, uint32_t* pu32DataLen)
{
    int strLength = strBase64.length();
    *pu32DataLen = 3*(strLength)/4;

    auto output = std::make_unique<uint8_t[]>(*pu32DataLen);

    const auto resultLen = EVP_DecodeBlock(output.get(), reinterpret_cast<const uint8_t*>(strBase64.c_str()), strBase64.length());
    if(resultLen != *pu32DataLen)
    {
        return 1;
    }

    //Little bug fix I added to fix incorrect length's because '==' signs are not considered in the output. -sspoke
    if (strLength > 3 && strBase64[strLength - 1] == '=' && strBase64[strLength - 2] == '='){
        *pu32DataLen = resultLen - 2;
    }
    else if (strLength > 2 && strBase64[strLength - 1] == '='){
        *pu32DataLen = resultLen - 1;
    }
    else{
        *pu32DataLen = resultLen;
    }

    *ppu8Data = output.release();
    return 0;
}

uint32_t base64Decode(const std::string& strBase64, uint8_t* pu8Data, uint32_t* pu32DataLen)
{
    int strLength = strBase64.length();
    auto expectLen = 3*(strBase64.length())/4;

    if(*pu32DataLen < expectLen){
        return 1;
    }

    const auto resultLen = EVP_DecodeBlock(pu8Data, reinterpret_cast<const uint8_t*>(strBase64.c_str()), strBase64.length());
    if(resultLen != expectLen)
    {
        return 1;
    }

    //Little bug fix I added to fix incorrect length's because '==' signs are not considered in the output. -sspoke
    if (strLength > 3 && strBase64[strLength - 1] == '=' && strBase64[strLength - 2] == '='){
        *pu32DataLen = resultLen - 2;
    }
    else if (strLength > 2 && strBase64[strLength - 1] == '='){
        *pu32DataLen = resultLen - 1;
    }
    else{
        *pu32DataLen = resultLen;
    }

    return 0;
}

#define IN 
#define OUT
#define INOUT

// TODO 调用流程确定   要协商密钥吗？ 协商一次密钥后续所有请求都通过这个通道吗？   通过 sendDataProtect 和 recvDataProtect 两个接口来收发数据吗？
// 服务器启动与密码服务平台协商密钥，然后通过 sendDataProtect 和 recvDataProtect 两个接口来收发数据？  服务关闭， release 会话？
// 这个通道能保持多久?
// 还是每次发送请求都要协商？
// 还是通过 http 请求
uint32_t InitReqA(IN const char* pszUserId, IN const char* pszInitReqAData, OUT char* pszInitRespBData);
uint32_t AuthReqA(IN const char* pszUserId, IN const char* pszInitAuthAData, OUT char* pszAuthRespBData);

// TODO 文档中有问题 算法表示字符串应该是 SM2， json中的 key 应该是 base64SecretKey
uint32_t ImportSymmKey(INOUT Key* pKey,IN uint32_t u32Period, IN const char* pszSymmAlg, IN uint32_t u32Length, IN const uint8_t* pu8SecretKey);
uint32_t DeleteKey(INOUT Key* pKey);
uint32_t EnableKey(INOUT Key* pKey);
uint32_t DisableKey(INOUT Key* pKey);

uint32_t GetRandom(uint32_t u32Length, OUT uint8_t* pu8Random);
uint32_t HashEx(IN const char* pszAlg, IN const uint8_t* pu8Data, uint32_t u32DataLen, OUT uint8_t** ppu8Hash);
uint32_t HAMCEx(IN const Key* pKey, IN const char* pszHmacAlg, IN const uint8_t* pu8Data, uint32_t u32DataLen, OUT uint8_t** ppu8Hmac);

// TODO 密码服务平台的接口有问题  IV 应该经过 base64编码， 文档中的没有体现
uint32_t SM4EncryptEx(IN const Key* pKey, IN const char* pszPaddingMode, IN const uint8_t* pu8Data, uint32_t u32DataLen, IN const uint8_t* pu8Iv, uint32_t u32IvLen, OUT uint8_t** ppu8CipherData);
uint32_t SM4DecryptEx(IN const Key* pKey, IN const char* pszPaddingMode, IN const uint8_t* pu8CipherData, uint32_t u32CipherDataLen, IN const uint8_t* pu8Iv, uint32_t u32IvLen, OUT uint8_t** ppu8Data);

uint32_t SignEx(IN const Key* pKey, IN const char* signAlg, IN const uint8_t* pu8Data, uint32_t u32DataLen, OUT uint8_t** ppu8Sign);
uint32_t Verify(IN const Key* pKey, IN const char* signAlg, IN const uint8_t* pu8Data, uint32_t u32DataLen, IN const uint8_t* pu8Sign, uint32_t u32SignLen);

uint32_t AsymEncryptEx(IN const Key* pKey, IN const char* encAlg, IN const uint8_t* pu8Data, uint32_t u32DataLen, OUT uint8_t** ppu8CipherData);
uint32_t AsymDecryptEx(IN const Key* pKey, IN const char* encAlg, IN const uint8_t* pu8CipherData, uint32_t u32CipherDataLen, OUT uint8_t** ppu8Data);


uint32_t InitReqA(const char* pszUserId, const char* pszInitReqAData, char* pszInitRespBData)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);

    nlohmann::json initReqAJson = {
        {
            "data", {
                {"userId", pszUserId},
                {"initReqAData", pszInitReqAData}
            }
        }
    };

    do{
        auto res = cli.Post("/api/v1/15843/parse/initReqA", initReqAJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string initRespBDataStr = initRespBJson["data"]["initRespBData"];
            strcpy(pszInitRespBData, initRespBDataStr.c_str());
            ret = 0;
        } else {
            ret = 1;
            break;
        }

    }while(0);

    return ret;
}

uint32_t AuthReqA(const char* pszUserId, const char* pszInitAuthAData, char* pszAuthRespBData)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);

    nlohmann::json AuthReqAJson = {
        {
            "data", {
                {"userId", pszUserId},
                {"initReqAData", pszInitAuthAData}
            }
        }
    };

    do{
        auto res = cli.Post("/api/v1/15843/parse/authReqA", AuthReqAJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string initRespBDataStr = initRespBJson["data"]["authRespBData"];
            strcpy(pszAuthRespBData, initRespBDataStr.c_str());
            ret = 0;
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t ImportSymmKey(Key* pkey, uint32_t u32Period, const char* pszSymmAlg, uint32_t u32Length, const uint8_t* pu8SecretKey)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);
    std::string bse64SecretKey;
    uint32_t keyLen = u32Length / 8;

    do{
        if(base64Encode(pu8SecretKey, keyLen, bse64SecretKey) != 0)
        {
            ret = 1;
            break;
        }

        nlohmann::json ImportSymmKeyJson = {
            {
                "data", {
                    {"alias", (pkey->pszAlias[0] == 0) ? nullptr : pkey->pszAlias},
                    {"version", pkey->u32Version},
                    {"period", u32Period},
                    {"symmAlg", pszSymmAlg},
                    {"length", u32Length},
                    // TODO base64SecretKey
                    {"base64SecretKey", bse64SecretKey}
                }
            }
        };
        auto res = cli.Post("/api/v1/kms/key/import", ImportSymmKeyJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }
            std::string alias = initRespBJson["data"]["alias"];
            if (alias.size() == 0 || alias.size() > MAX_ALIAS_LEN - 1)
            {
                ret = 1;
                break;
            }
            // check size ?
            strcpy(pkey->pszAlias, alias.c_str());
            
            ret = 0;
        } else {
            ret = -1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t DisableKey(Key* pKey)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);


    do{
        if(pKey->pszAlias[0] == 0)
        {
            ret = 1;
            break;
        }
        nlohmann::json disableKeyJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version}
                }
            }
        };

        auto res = cli.Put("/api/v1/kms/key/disable", disableKeyJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }
            ret = 0;
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t EnableKey(Key* pKey)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);

    do{
        if(pKey->pszAlias[0] == 0)
        {
            ret = 1;
            break;
        }
        nlohmann::json enableKeyJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version}
                }
            }
        };

        auto res = cli.Put("/api/v1/kms/key/enable", enableKeyJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }
            ret = 0;
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t DeleteKey(Key* pKey)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);

    do{
        if(pKey->pszAlias[0] == 0)
        {
            ret = 1;
            break;
        }
        nlohmann::json deleteKeyJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version}
                }
            }
        };

        auto res = cli.Put("/api/v1/kms/key/revoke", deleteKeyJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }
            ret = 0;
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t GetRandom(uint32_t u32Length, uint8_t* pu8Random)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);

    do{
        nlohmann::json getRandomJson = {
            {
                "data", {
                    {"length", u32Length}
                }
            }
        };
        auto res = cli.Post("/api/v1/crypto/random", getRandomJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64RandomStr = initRespBJson["data"]["random"];
            ret = base64Decode(base64RandomStr, pu8Random, &u32Length);
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t HashEx(const char* pszAlg, const uint8_t* pu8Data, uint32_t u32DataLen, uint8_t** ppu8Hash, uint32_t* pu32HashLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);
    std::string base64DataStr; // TODO base64 encode needed

    do{
        ret = base64Encode(pu8Data, u32DataLen, base64DataStr);
        nlohmann::json hashJson = {
            {
                "data", {
                    {"alg", pszAlg},
                    {"data", base64DataStr}
                }
            }
        };

        auto res = cli.Post("/api/v1/crypto/hash", hashJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64HashStr = initRespBJson["data"]["hashValue"];
            ret = base64DecodeEx(base64HashStr, ppu8Hash, pu32HashLen);
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t HAMCEx(const Key* pKey, const char* pszHmacAlg, const uint8_t* pu8Data, uint32_t u32DataLen, uint8_t** ppu8Hmac, uint32_t* pu32HmacLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);

    std::string base64Data;



    do{
        ret = base64Encode(pu8Data, u32DataLen, base64Data);
        if(ret != 0){
            break;
        }
        nlohmann::json HAMCJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version},
                    {"hmacAlg", pszHmacAlg}, // "HMACSM3"
                    {"data", base64Data}
                }
            }
        };

        auto res = cli.Post("/api/v1/crypto/hmac", HAMCJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64HmacStr = initRespBJson["data"]["hmacValue"];
            ret = base64DecodeEx(base64HmacStr, ppu8Hmac, pu32HmacLen);
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t SM4EncryptEx(const Key* pKey, const char* pszPaddingMode, const uint8_t* pu8Data, uint32_t u32DataLen, const uint8_t* pu8Iv, uint32_t u32IvLen, uint8_t** ppu8CipherData, uint32_t* pu32CipherDataLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);
    std::string base64Data;
    std::string base64Iv;


    do{
        if(base64Encode(pu8Data, u32DataLen, base64Data) != 0 ||
            base64Encode(pu8Iv, u32IvLen, base64Iv) != 0)
        {
            ret = 1;
            break;
        }

        nlohmann::json SM4EncryptJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version},
                    {"paddingMode", pszPaddingMode},
                    {"base64PlainData", base64Data},
                    {"base64Iv", base64Iv}
                }
            }
        };

        auto res = cli.Post("/api/v1/crypto/encrypt", SM4EncryptJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64CipherStr = initRespBJson["data"]["base64CipherData"];
            ret = base64DecodeEx(base64CipherStr, ppu8CipherData, pu32CipherDataLen);
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t SM4DecryptEx(const Key* pKey, const char* pszPaddingMode, const uint8_t* pu8CipherData, uint32_t u32CipherDataLen, const uint8_t* pu8Iv, uint32_t u32IvLen, uint8_t** ppu8Data, uint32_t* pu32DataLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);
    std::string base64CipherData;
    std::string base64Iv;

    do{
        if(base64Encode(pu8CipherData, u32CipherDataLen, base64CipherData) != 0 ||
            base64Encode(pu8Iv, u32IvLen, base64Iv) != 0)
        {
            ret = 1;
            break;
        }

        nlohmann::json SM4DecryptJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version},
                    {"paddingMode", pszPaddingMode},
                    {"base64CipherData", base64CipherData},
                    {"base64Iv", base64Iv}
                }
            }
        };
        auto res = cli.Post("/api/v1/crypto/decrypt", SM4DecryptJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64Data = initRespBJson["data"]["base64PlainData"];
            ret = base64DecodeEx(base64Data, ppu8Data, pu32DataLen);
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t SignEx(const Key* pKey, const char* signAlg, const uint8_t* pu8Data, uint32_t u32DataLen, uint8_t** ppu8Sign, uint32_t* pu32SignLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);

    std::string base64Data;



    do{
        if(base64Encode(pu8Data, u32DataLen, base64Data) != 0)
        {
            ret = 1;
            break;
        }

        nlohmann::json signJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version},
                    {"signAlg", signAlg},  // "SM3withSM2"
                    {"base64PlainData", base64Data}
                }
            }
        };
        auto res = cli.Post("/api/v1/crypto/sign", signJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64SignDataStr = initRespBJson["data"]["base64SignData"];
            ret = base64DecodeEx(base64SignDataStr, ppu8Sign, pu32SignLen);
            ret = 0;
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t Verify(const Key* pKey, const char* signAlg, const uint8_t* pu8Data, uint32_t u32DataLen, const uint8_t* pu8Sign, uint32_t u32SignLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);
    std::string base64Data;
    std::string base64Sign;

    do{
        if(base64Encode(pu8Data, u32DataLen, base64Data) != 0 ||
            base64Encode(pu8Sign, u32SignLen, base64Sign) != 0)
        {
            ret = 1;
            break;
        }

        nlohmann::json verifyJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version},
                    {"signAlg", signAlg},  // "SM3withSM2"
                    {"base64PlainData", base64Data},
                    {"base64SignData", base64Sign}
                }
            }
        };
        auto res = cli.Post("/api/v1/crypto/verify", verifyJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }
            ret = 0;
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t AsymEncryptEx(const Key* pKey, const char* encAlg, const uint8_t* pu8Data, uint32_t u32DataLen, uint8_t** ppu8CipherData, uint32_t* pu32CipherDataLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);
    std::string base64Data;

    do{
        if(base64Encode(pu8Data, u32DataLen, base64Data) != 0)
        {
            ret = 1;
            break;
        }

        nlohmann::json encryptJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version},
                    {"encAlg", encAlg},  // "SM3withSM2"
                    {"base64PlainData", base64Data}
                }
            }
        };
        // TODO encrypt --> asymencrypt
        auto res = cli.Post("/api/v1/crypto/encrypt", encryptJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64CipherDataStr = initRespBJson["data"]["base64CipherData"];
            ret = base64DecodeEx(base64CipherDataStr, ppu8CipherData, pu32CipherDataLen);
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

uint32_t AsymDecryptEx(const Key* pKey, const char* encAlg, const uint8_t* pu8CipherData, uint32_t u32CipherDataLen, uint8_t** ppu8Data, uint32_t* pu32DataLen)
{
    uint32_t ret;
    std::string url = "http://" + ip + port;
    httplib::Client cli(url);
    std::string base64CipherData;

    do{
        if(base64Encode(pu8CipherData, u32CipherDataLen, base64CipherData) != 0)
        {
            ret = 1;
            break;
        }

        nlohmann::json decryptJson = {
            {
                "data", {
                    {"alias", pKey->pszAlias},
                    {"version", pKey->u32Version},
                    {"encAlg", encAlg},  // "SM3withSM2"
                    {"base64CipherData", base64CipherData}
                }
            }
        };
        // TODO decrypt --> asymdecrypt
        auto res = cli.Post("/api/v1/crypto/decrypt", decryptJson.dump(), "application/json");
        if (res && res->status == 200) {
            nlohmann::json initRespBJson = nlohmann::json::parse(res->body);
            if(initRespBJson["code"] != 20000)
            {
                ret = 1;
                break;
            }

            std::string base64Data = initRespBJson["data"]["base64PlainData"];
            ret = base64DecodeEx(base64Data, ppu8Data, pu32DataLen);
        } else {
            ret = 1;
            break;
        }
    }while(0);

    return ret;
}

void test_json()
{
    nlohmann::json j = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
            {"currency", "USD"},
            {"value", 42.99}
        }}
    };

    std::cout << j.dump(4) << std::endl;

    std::cout << j["pi"] << std::endl;
}

void test_base64()
{
    int ret = 0;
    auto base64_str = "VQxkoqbZKgn/voI7TEFYVMY+O7g+ahEkeMlJQejBBPk=";
    auto hex_str = "550c64a2a6d92a09ffbe823b4c415854c63e3bb83e6a112478c94941e8c104f9";

    int origin_data_len = 0;
    auto origin_data = hexStringToBytes(hex_str, &origin_data_len, 0);

    std::string out_base64_str;
    ret = base64Encode(origin_data, origin_data_len, out_base64_str);

    if(ret != 0 || base64_str != out_base64_str){
        std::cout << "base64Encode failed!!!" << std::endl;
    }else{
        std::cout << "base64Encode success!!!" << std::endl;
    }

    uint8_t* out_data = nullptr;
    uint32_t out_data_len = 0;
    base64DecodeEx(base64_str, &out_data, &out_data_len);

    if(ret != 0 || origin_data_len != out_data_len || memcmp(origin_data, out_data, out_data_len) != 0){
        std::cout << "base64DecodeEx failed!!!" << std::endl;
    }else{
        std::cout << "base64DecodeEx success!!!" << std::endl;
    }

    free(out_data);
    free(origin_data);
}

int main() {
    // test_httplib();
    // test_json();
    test_base64();
    int *a;
    // right
    void* b = a;
    // error
    // a = (void*)0;
    return 0;
}