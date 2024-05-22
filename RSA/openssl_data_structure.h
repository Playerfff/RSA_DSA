//
// Created by huawei on 2024/5/22.
//

#ifndef RSA_DSA_OPENSSL_DATA_STRUCTURE_H
#define RSA_DSA_OPENSSL_DATA_STRUCTURE_H

struct rsa_st {
    const RSA_METHOD *meth;
    BIGNUM *n;   // n=p*q
    BIGNUM *e;   // 加密指数
    BIGNUM *d;   // 私钥
    BIGNUM *p;   // 大素数p
    BIGNUM *q;   // 大素数p
    BIGNUM *dmpl;// d mod (p-1)
    BIGNUM *dmql;// d mod (q-1)
    BIGNUM *iqmp;// (inverse of q) mod p
    /* others */
};

#endif//RSA_DSA_OPENSSL_DATA_STRUCTURE_H
