#include "utils.h"
#include <stdio.h>
#include <string.h>

void swap32s(uint32_t *buf, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        *buf = bswap32(*buf);
        buf++;
    }
}

void swap24s(uint8_t *buf, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        uint8_t v0 = buf[0];
        uint8_t v2 = buf[2];
        buf[0] = v2;
        buf[2] = v0;
        buf   += 3;
    }
}

void swap16s(uint16_t *buf, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        *buf = bswap16(*buf);
        buf++;
    }
}

const int TILE_ORDER[] = { 0,   1,  8,  9,  2,  3, 10, 11,
                           16, 17, 24, 25, 18, 19, 26, 27,
                           4,   5, 12, 13,  6,  7, 14, 15,
                           20, 21, 28, 29, 22, 23, 30, 31,
                           32, 33, 40, 41, 34, 35, 42, 43,
                           48, 49, 56, 57, 50, 51, 58, 59,
                           36, 37, 44, 45, 38, 39, 46, 47,
                           52, 53, 60, 61, 54, 55, 62, 63 };

template <typename T>
void decode_block(const T *in, size_t width, size_t height, T *out)
{
    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            for (int i = 0; i < 64; i++)
            {
                size_t order = TILE_ORDER[i];
                size_t x     = order & 7;
                size_t y     = order >> 3;
                out[(ty + y) * width + tx + x] = *in++;
            }
        }
    }
}

void decode_block8(const uint8_t *in, size_t width, size_t height, uint8_t *out)
{
    return decode_block(in, width, height, out);
}

void decode_block16(const uint16_t *in, size_t width, size_t height, uint16_t *out)
{
    return decode_block(in, width, height, out);
}

void decode_block24(const uint8_t *in, size_t width, size_t height, uint8_t *out)
{
    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            for (int i = 0; i < 64; i++)
            {
                size_t order = TILE_ORDER[i];
                size_t x     = order & 7;
                size_t y     = order >> 3;

                uint8_t *op = out + ((ty + y) * width + tx + x) * 3;
                *op++ = *in++;
                *op++ = *in++;
                *op++ = *in++;
            }
        }
    }
}

void decode_block32(const uint32_t *in, size_t width, size_t height, uint32_t *out)
{
    return decode_block(in, width, height, out);
}

template <typename T>
void encode_block(const T *in, size_t width, size_t height, T *out)
{
    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            for (int i = 0; i < 64; i++)
            {
                size_t order = TILE_ORDER[i];
                size_t x     = order & 7;
                size_t y     = order >> 3;
                *out++ = in[(ty + y) * width + tx + x];
            }
        }
    }
}

void encode_block8(const uint8_t *in, size_t width, size_t height, uint8_t *out)
{
    return encode_block(in, width, height, out);
}

void encode_block16(const uint16_t *in, size_t width, size_t height, uint16_t *out)
{
    return encode_block(in, width, height, out);
}

void encode_block24(const uint8_t *in, size_t width, size_t height, uint8_t *out)
{
    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            for (int i = 0; i < 64; i++)
            {
                size_t order = TILE_ORDER[i];
                size_t x     = order & 7;
                size_t y     = order >> 3;

                const uint8_t *ip = in + ((ty + y) * width + tx + x) * 3;
                *out++ = *ip++;
                *out++ = *ip++;
                *out++ = *ip++;
            }
        }
    }
}

void encode_block32(const uint32_t *in, size_t width, size_t height, uint32_t *out)
{
    return encode_block(in, width, height, out);
}
