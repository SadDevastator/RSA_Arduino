#ifndef HAL_RSA_H
#define HAL_RSA_H

#include <stdint.h>
#include <stddef.h>

uint32_t modexp(uint32_t base, uint32_t exp, uint32_t mod);

uint32_t rsa_encrypt_uint(uint32_t m, uint32_t e, uint32_t n);
uint32_t rsa_decrypt_uint(uint32_t c, uint32_t d, uint32_t n);

size_t rsa_encrypt_bytes(const uint8_t *in, size_t in_len, uint16_t *out, uint32_t e, uint32_t n);

size_t rsa_decrypt_bytes(const uint16_t *in, size_t in_len, uint8_t *out, uint32_t d, uint32_t n);

#endif
