#ifndef __BASE64_H
#define __BASE64_H

#include <stdint.h>


#define BASE64_ENCODE_OUT_SIZE(inlen) ((unsigned int)((((inlen) + 2) / 3) * 4 + 1))
#define BASE64_DECODE_OUT_SIZE(inlen) ((unsigned int)(((inlen) / 4) * 3))

// return value is the encoding length
uint32_t base64_encode(const uint8_t *in, uint32_t inlen, uint8_t *out);

// return value is the decoding length
uint32_t base64_decode(const uint8_t *in, uint32_t inlen, uint8_t *out);


#endif //__BASE64_H
