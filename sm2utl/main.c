#include "openssl/bio.h"
#include "openssl/evp.h"
#include <openssl/ssl.h>
#include <openssl/sm2.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

BIO *out = NULL;

int sm2DecryptWithGmssl(
                       unsigned char *inData,
                       unsigned long inDataLen,
                       const char *priFile,
                       uint8_t *buf,
                       size_t *maxLen)
{
    int resultCode = 0;
    BIO *pri_bio = NULL;
    EC_KEY *ec_key = NULL;
    int res = 0;

    //判断输入参数是否为空
    if (inData == NULL || inDataLen == 0 || priFile == NULL) {
        resultCode = -1;
        goto err;
    }
    pri_bio = BIO_new_file(priFile, "r");
    if (!pri_bio) {
        BIO_printf(out, "open prikey file error!\n");
        goto err;
    }
    ec_key = PEM_read_bio_ECPrivateKey(pri_bio, &ec_key, NULL, NULL);
    if (!ec_key) {
        BIO_printf(out, "open prikey error!\n");
        goto err;
    }

    //调用SM2加密
    resultCode = SM2_decrypt(ec_key, EVP_sm3(), inData, inDataLen, buf, maxLen);
    if (!res) {
        goto err;
    }

err:
    if (ec_key != NULL) {
        EC_KEY_free(ec_key);
    }

    if (pri_bio)
        BIO_free(pri_bio);

    return resultCode;
}

/**
 使用gmssl SM2加密

 @param inData 需要加密的数据
 @param inDataLen 需要加密的数据长度
 @param pubKey 公钥（point2oct）
 @param pubKeyLen 公钥长度
 @param encryptData 加密后的数据
 @return 0：成功/非0：失败
 */
 int sm2EncryptWithGmssl(
                       unsigned char *inData,
                       unsigned long inDataLen,
                       const char *pubFile,
                       uint8_t *buf,
                       size_t* maxLen)
{
    int resultCode = 0;
    //公钥
    EC_KEY *ec_key = NULL;
    BIO *pub_bio = NULL;

    //判断输入参数是否为空
    if (inData == NULL || inDataLen == 0 || pubFile == NULL) {
        resultCode = -1;
        goto err;
    }
    pub_bio = BIO_new_file(pubFile, "r");
    if (!pub_bio) {
        BIO_printf(out, "open pubkey file error!\n");
        goto err;
    }
    ec_key = PEM_read_bio_EC_PUBKEY(pub_bio, &ec_key, NULL, NULL);
    if (!ec_key) {
        BIO_printf(out, "open pubkey error!\n");
        goto err;
    }

    //调用SM2加密
    resultCode = SM2_encrypt(ec_key, EVP_sm3(), inData, inDataLen, buf, maxLen);
    if (!resultCode) {
        goto err;
    }

err:
    if (ec_key != NULL) {
        EC_KEY_free(ec_key);
    }

    if (pub_bio)
        BIO_free(pub_bio);

    return resultCode;
}

int main(int argc, char** argv) {
    char intext[1024] = "out = BIO_new_fp(stdout, BIO_NOCLOSE | BIO_FP_TEXT)";
    uint8_t cip[1024] = {0};
    uint8_t pla[1024] = {0};
    size_t cipLen = 0;
    size_t plaLen = 0;
    size_t inLen = strlen(intext);
    out = BIO_new_fp(stdout, BIO_NOCLOSE | BIO_FP_TEXT);

    int res = -1;

    if (!out) {
        printf("open bio out faild");
        return -1;
    }

    if (argc < 3) {
        BIO_printf(out, "Usage: %s <private key> <public key>.\n", argv[0]);
        goto end;
    }

    sm2EncryptWithGmssl((uint8_t*)intext, inLen, argv[1], cip, &cipLen);
    sm2DecryptWithGmssl(cip, cipLen, argv[2], pla, &plaLen);
    
    if (0 == strcmp(intext, (const char*)pla)) {
        BIO_printf(out, "key pair success.\n");
        res = 0;
    }

end:
    if (out)
        BIO_free(out);

    return res;
}
