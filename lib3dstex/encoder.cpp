#include <stdint.h>
#include <string.h>
#include "rg_etc1.h"
#include "tex3ds.h"
#include "utils.h"

static bool gEtc1Inited = false;

template <typename T>
void encode_block(T *in, size_t width, size_t height, T *out)
{
    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width ; tx += 8)
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

void encode_block32(uint32_t *in, size_t width, size_t height, uint32_t *out)
{
    return encode_block(in, width, height, out);
}

void encode_block16(uint16_t *in, size_t width, size_t height, uint16_t *out)
{
    return encode_block(in, width, height, out);
}

void encode_block8(uint8_t *in, size_t width, size_t height, uint8_t *out)
{
    return encode_block(in, width, height, out);
}

void encode_rgba8(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    uint8_t *tmp = new uint8_t[width * height * 4];

    memcpy(tmp, inbuf, width * height * 4);
    swap32s((uint32_t *)tmp, width * height);
    encode_block32((uint32_t *)tmp, width, height, (uint32_t *)outbuf);
    delete[]tmp;
}

void encode_etc1(uint8_t* inbuf, size_t width, size_t height, uint8_t* outbuf)
{
    if (!gEtc1Inited)
    {
        rg_etc1::pack_etc1_block_init();
        gEtc1Inited = true;
    }

    uint32_t* tmp = new uint32_t[width * height];

    uint8_t* p = inbuf;
    for (size_t i = 0; i < width * height; i++)
    {
        uint8_t r = *p++;
        uint8_t g = *p++;
        uint8_t b = *p++;
        tmp[i] = (r << 24) | (g << 16) | (b << 8) | 0xff;
    }

    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            uint32_t  pixels[64];
            for (int i = 0; i < 64; i++)
            {
                size_t order = ETC1_TILE_ORDER[i];
                size_t x = order & 7;
                size_t y = order >> 3;
                pixels[i] = tmp[(ty + y) * width + tx + x];
            }

            for (int i = 0; i < 4; i++)
            {
            }
        }
    }

    delete[]tmp;
}

CODEC_FUNC ENCODE_FUNCTIONS[] = { encode_rgba8,
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
                                  NULL,
                                  NULL,
                                  NULL,
};

EXPORT size_t get_encode_size(size_t width, size_t height, TEXTURE_FORMAT format)
{
    return format < FORMAT_NUM ? (size_t)(width * height * ENCODE_RATIO[format]) : 0;
}

EXPORT void encode(uint8_t *inbuf, size_t width, size_t height, TEXTURE_FORMAT format, uint8_t *outbuf)
{
    if (format < FORMAT_NUM)
    {
        return ENCODE_FUNCTIONS[format](inbuf, width, height, outbuf);
    }
}
