#include "RSA.h"
#include <Arduino.h>

uint32_t modexp(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp > 0)
    {
        if (exp & 1)
        {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint32_t)result;
}

uint32_t rsa_encrypt_uint(uint32_t m, uint32_t e, uint32_t n)
{
    return modexp(m, e, n);
}

uint32_t rsa_decrypt_uint(uint32_t c, uint32_t d, uint32_t n)
{
    return modexp(c, d, n);
}

size_t rsa_encrypt_bytes(const uint8_t *in, size_t in_len, uint16_t *out, uint32_t e, uint32_t n)
{
    for (size_t i = 0; i < in_len; ++i)
    {
        uint32_t m = in[i];
        uint32_t c = rsa_encrypt_uint(m, e, n);
        out[i] = (uint16_t)c;
    }
    return in_len;
}

size_t rsa_decrypt_bytes(const uint16_t *in, size_t in_len, uint8_t *out, uint32_t d, uint32_t n)
{
    for (size_t i = 0; i < in_len; ++i)
    {
        uint32_t c = in[i];
        uint32_t m = rsa_decrypt_uint(c, d, n);
        out[i] = (uint8_t)(m & 0xFF);
    }
    return in_len;
}
