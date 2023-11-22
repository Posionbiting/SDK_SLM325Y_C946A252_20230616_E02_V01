/*
 *the mbedtls configuration file of ec616_0h00 basic
 */

#ifndef MBEDTLS_CONFIG_HTTPS_H
#define MBEDTLS_CONFIG_HTTPS_H

/* mbed TLS feature support */
#define MBEDTLS_SSL_PROTO_SSL3
#define MBEDTLS_SSL_PROTO_TLS1
#define MBEDTLS_SSL_PROTO_TLS1_1
#define MBEDTLS_SSL_PROTO_TLS1_2

#define MBEDTLS_THREADING_C
#define MBEDTLS_THREADING_IMPL

#define MBEDTLS_USE_RAND_API_ENTROPY

//#define MBEDTLS_CMAC_C
//#define MBEDTLS_ECJPAKE_C

#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_CRL_PARSE_C
#define MBEDTLS_X509_CSR_PARSE_C

#define MBEDTLS_SSL_SERVER_NAME_INDICATION


/* SSL CIPHERSUITES */
#define MBEDTLS_DES_C
#define MBEDTLS_CCM_C
#define MBEDTLS_GCM_C
//#define MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED
#define MBEDTLS_CIPHER_MODE_CBC
//#define MBEDTLS_CIPHER_MODE_CFB
//#define MBEDTLS_CIPHER_MODE_CTR

/* EC */
#define MBEDTLS_ECP_C
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
//#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
//#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
//#define MBEDTLS_ECP_DP_BP384R1_ENABLED
//#define MBEDTLS_ECP_DP_BP512R1_ENABLED
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
#define MBEDTLS_ECP_NIST_OPTIM

//#define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH

#define MBEDTLS_DEBUG_C

#endif /* MBEDTLS_CONFIG_HTTPS_H */
