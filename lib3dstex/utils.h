#pragma once
#include <stdint.h>
#include <stdlib.h>

// copy from https://gist.github.com/jtbr/7a43e6281e6cca353b33ee501421860c
#if defined(_MSC_VER) // needs to be first because msvc doesn't short-circuit after failing defined(__has_builtin)
#  define bswap16(x)    _byteswap_ushort((x))
#  define bswap32(x)    _byteswap_ulong((x))
#  define bswap64(x)    _byteswap_uint64((x))
#elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#  define bswap16(x)    __builtin_bswap16((x))
#  define bswap32(x)    __builtin_bswap32((x))
#  define bswap64(x)    __builtin_bswap64((x))
#elif defined(__has_builtin) && __has_builtin(__builtin_bswap64)  /* for clang; gcc 5 fails on this and && shortcircuit fails; must be after GCC check */
#  define bswap16(x)    __builtin_bswap16((x))
#  define bswap32(x)    __builtin_bswap32((x))
#  define bswap64(x)    __builtin_bswap64((x))
#else
/* even in this case, compilers often optimize by using native instructions */
static inline uint16_t bswap16(uint16_t x)
{
    return ((x >> 8) & 0xffu) | ((x & 0xffu) << 8);
}

static inline uint32_t bswap32(uint32_t x)
{
    return ((x & 0xff000000u) >> 24) |
           ((x & 0x00ff0000u) >> 8) |
           ((x & 0x0000ff00u) << 8) |
           ((x & 0x000000ffu) << 24);
}

static inline uint64_t bswap64(uint64_t x)
{
    return ((x & 0xff00000000000000ull) >> 56) |
           ((x & 0x00ff000000000000ull) >> 40) |
           ((x & 0x0000ff0000000000ull) >> 24) |
           ((x & 0x000000ff00000000ull) >> 8) |
           ((x & 0x00000000ff000000ull) << 8) |
           ((x & 0x0000000000ff0000ull) << 24) |
           ((x & 0x000000000000ff00ull) << 40) |
           ((x & 0x00000000000000ffull) << 56);
}

#endif

extern void swap16s(uint16_t *buf, size_t size);
extern void swap24s(uint8_t *buf, size_t size);
extern void swap32s(uint32_t *buf, size_t size);

extern void decode_block8(const uint8_t *in, size_t width, size_t height, uint8_t *out);
extern void decode_block16(const uint16_t *in, size_t width, size_t height, uint16_t *out);
extern void decode_block24(const uint8_t *in, size_t width, size_t height, uint8_t *out);
extern void decode_block32(const uint32_t *in, size_t width, size_t height, uint32_t *out);

extern void encode_block8(const uint8_t *in, size_t width, size_t height, uint8_t *out);
extern void encode_block16(const uint16_t *in, size_t width, size_t height, uint16_t *out);
extern void encode_block24(const uint8_t *in, size_t width, size_t height, uint8_t *out);
extern void encode_block32(const uint32_t *in, size_t width, size_t height, uint32_t *out);
