/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#ifndef MG_INFRA_SHA1_H_
#define MG_INFRA_SHA1_H_
#include <stdint.h>

void mg_utils_sha1(const unsigned char *input, uint32_t ilen, unsigned char output[20]);
void mg_utils_hmac_sha1(const char *msg, int msg_len, char *digest, const char *key, int key_len);
void mg_utils_hmac_sha1_hex(const char *msg, int msg_len, char *digest, const char *key, int key_len);

#endif
