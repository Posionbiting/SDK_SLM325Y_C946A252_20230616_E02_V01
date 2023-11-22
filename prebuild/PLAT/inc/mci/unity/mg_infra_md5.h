/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */
#ifndef MG_INFRA_MD5_H_
#define MG_INFRA_MD5_H_
#include <stdint.h>
/**
 * \brief          Output = MD5( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   MD5 checksum result
 */
void mg_utils_md5(const unsigned char *input, uint32_t ilen, unsigned char output[16]);

void mg_utils_hmac_md5(const char *msg, int msg_len, char *digest, const char *key, int key_len);

#endif

