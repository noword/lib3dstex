#pragma once

#include <stdint.h>

#ifdef _MSC_VER

#include <stdlib.h>
#define bswap_32(x)    _byteswap_ulong(x)
#define bswap_64(x)    _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bswap_32(x)    OSSwapInt32(x)
#define bswap_64(x)    OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_32(x)    BSWAP_32(x)
#define bswap_64(x)    BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_32(x)    bswap32(x)
#define bswap_64(x)    bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_32(x)    swap32(x)
#define bswap_64(x)    swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x)    bswap32(x)
#define bswap_64(x)    bswap64(x)
#endif

#else

#include <byteswap.h>

#endif

extern void swap32s(uint32_t* buf, size_t size);
extern void swap24s(uint8_t* buf, size_t size);

extern void decode_block8(uint8_t* in, size_t width, size_t height, uint8_t* out);
extern void decode_block16(uint16_t* in, size_t width, size_t height, uint16_t* out);
extern void decode_block24(uint8_t* in, size_t width, size_t height, uint8_t* out);
extern void decode_block32(uint32_t* in, size_t width, size_t height, uint32_t* out);

extern void encode_block8(uint8_t* in, size_t width, size_t height, uint8_t* out);
extern void encode_block16(uint16_t* in, size_t width, size_t height, uint16_t* out);
extern void encode_block24(uint8_t* in, size_t width, size_t height, uint8_t* out);
extern void encode_block32(uint32_t* in, size_t width, size_t height, uint32_t* out);
