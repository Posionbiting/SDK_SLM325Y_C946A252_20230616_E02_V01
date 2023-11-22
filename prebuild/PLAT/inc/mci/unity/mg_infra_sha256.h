/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#ifndef MG_IOTX_COMMON_SHA256_H_
#define MG_IOTX_COMMON_SHA256_H_

#include <stdint.h>

#define SHA256_DIGEST_LENGTH            (32)
#define SHA256_BLOCK_LENGTH             (64)
#define SHA256_SHORT_BLOCK_LENGTH       (SHA256_BLOCK_LENGTH - 8)
#define SHA256_DIGEST_STRING_LENGTH     (SHA256_DIGEST_LENGTH * 2 + 1)


/**
 * \brief          Output = SHA-256( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SHA-256 checksum result
 */
void mg_utils_sha256(const uint8_t *input, uint32_t ilen, uint8_t output[32]);

void mg_utils_hmac_sha256(const uint8_t *msg, uint32_t msg_len, const uint8_t *key, uint32_t key_len, uint8_t output[32]);

#endif

