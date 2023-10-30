#include <stdint.h>
#include "rg_etc1.h"
#include "tex3ds.h"
#include "utils.h"

template <typename T>
void decode_block(T *in, size_t width, size_t height, T *out)
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

void decode_block32(uint32_t *in, size_t width, size_t height, uint32_t *out)
{
    return decode_block(in, width, height, out);
}

void decode_block16(uint16_t *in, size_t width, size_t height, uint16_t *out)
{
    return decode_block(in, width, height, out);
}

void decode_block8(uint8_t *in, size_t width, size_t height, uint8_t *out)
{
    return decode_block(in, width, height, out);
}

void decode_block24(uint8_t *in, size_t width, size_t height, uint8_t *out)
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

void decode_rgba8(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block32((uint32_t *)inbuf, width, height, (uint32_t *)outbuf);
    swap32s((uint32_t *)outbuf, width * height);
}

void decode_rgb8(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block24(inbuf, width, height, outbuf);
    uint8_t *p = outbuf;
    for (int i = 0; i < width * height; i++)
    {
        uint8_t r = p[0];
        uint8_t g = p[2];
        p[0] = g;
        p[2] = r;
        p   += 3;
    }
}

void decode_etc1(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    uint64_t *in64  = (uint64_t *)inbuf;
    uint32_t *out32 = (uint32_t *)outbuf;

    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            uint32_t  pixels[64];
 
            for (int i = 0; i < 4; i++)
            {
                uint64_t tmp = bswap_64(*in64++);
                rg_etc1::unpack_etc1_block(&tmp, pixels + i * 16, false);
            }

            for (int i = 0; i < 64; i++)
            {
                size_t order = ETC1_TILE_ORDER[i];
                size_t x     = order & 7;
                size_t y     = order >> 3;

                uint8_t *out = (uint8_t *)(out32 + (ty + y) * width + tx + x);
                uint8_t *in  = (uint8_t *)&pixels[i];

                *out++ = *in++;
                *out++ = *in++;
                *out++ = *in++;
            }
        }
    }
}

void decode_etc1a4(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
}

CODEC_FUNC DECODE_FUNCTIONS[] = { decode_rgba8,
                                  decode_rgb8,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  decode_etc1,
                                  NULL,
};

EXPORT size_t get_decode_size(size_t width, size_t height, TEXTURE_FORMAT format)
{
    return format < FORMAT_NUM ? width * height * DECODE_RATIO[format]:0;
}

EXPORT void decode(uint8_t *inbuf, size_t width, size_t height, TEXTURE_FORMAT format, uint8_t *outbuf)
{
    if (format < FORMAT_NUM)
    {
        return DECODE_FUNCTIONS[format](inbuf, width, height, outbuf);
    }
}
