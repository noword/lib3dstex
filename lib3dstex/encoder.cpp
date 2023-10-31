#include <stdint.h>
#include <string.h>
#include "rg_etc1.h"
#include "tex3ds.h"
#include "utils.h"

static bool gEtc1Inited = false;

void encode_rgba8(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    uint8_t *tmp = new uint8_t[width * height * 4];

    memcpy(tmp, inbuf, width * height * 4);
    swap32s((uint32_t *)tmp, width * height);
    encode_block32((uint32_t *)tmp, width, height, (uint32_t *)outbuf);
    delete[]tmp;
}

void encode_rgb8(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    uint8_t *tmp = new uint8_t[width * height * 3];
    memcpy(tmp, inbuf, width * height * 3);
    swap24s(tmp, width * height);
    encode_block24(tmp, width, height, outbuf);
    delete[]tmp;
}

void encode_etc1(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    if (!gEtc1Inited)
    {
        rg_etc1::pack_etc1_block_init();
        gEtc1Inited = true;
    }

    rg_etc1::etc1_pack_params params;
    params.clear();

    uint32_t *tmp   = new uint32_t[width * height];
    uint8_t * p     = inbuf;
    uint64_t *out64 = (uint64_t *)outbuf;

    for (size_t i = 0; i < width * height; i++)
    {
        uint8_t r = *p++;
        uint8_t g = *p++;
        uint8_t b = *p++;
        tmp[i] = (b << 16) | (g << 8) | r | 0xff000000;
    }

    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            uint32_t pixels[64];
            for (int i = 0; i < 64; i++)
            {
                size_t order = ETC1_TILE_ORDER[i];
                size_t x     = order & 7;
                size_t y     = order >> 3;
                pixels[i] = tmp[(ty + y) * width + tx + x];
            }
            unsigned int *p = (unsigned int *)pixels;
            for (int i = 0; i < 4; i++)
            {
                rg_etc1::pack_etc1_block(out64, p + i * 16, params);
                *out64 = bswap_64(*out64);
                out64++;
            }
        }
    }

    delete[]tmp;
}

void encode_etc1a4(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    if (!gEtc1Inited)
    {
        rg_etc1::pack_etc1_block_init();
        gEtc1Inited = true;
    }

    rg_etc1::etc1_pack_params params;
    params.clear();

    uint32_t *in32  = (uint32_t *)inbuf;
    uint64_t *out64 = (uint64_t *)outbuf;

    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            uint32_t pixels[64];
            uint32_t  alpha[64] = { 0 };
            for (int i = 0; i < 64; i++)
            {
                size_t   order = ETC1_TILE_ORDER[i];
                size_t   x     = order & 7;
                size_t   y     = order >> 3;
                uint32_t p     = in32[(ty + y) * width + tx + x];
                pixels[i] = (p & 0xffffff) | 0xff000000;
                alpha[i]  = p >> 28;
            }

            unsigned int *p = (unsigned int *)pixels;
            for (int i = 0; i < 4; i++)
            {
                *out64 = 0;
                for (int j = 15; j >= 0; j--)
                {
                    *out64 <<= 4;
                    *out64  |= alpha[i * 16 + ETC1_ALPHA_ORDER[j]];
                }
                out64++;

                rg_etc1::pack_etc1_block(out64, p + i * 16, params);
                *out64 = bswap_64(*out64);
                out64++;
            }
        }
    }
}

const CODEC_FUNC ENCODE_FUNCTIONS[] = { encode_rgba8,
                                        encode_rgb8,
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
                                        encode_etc1,
                                        encode_etc1a4,
};

const float ENCODE_RATIO[] = { 4,   // RGBA_8888
                               3,   // RGB_888
                               2,   // RGBA_5551
                               2,   // RGB_565
                               2,   // RGBA_4444
                               2,   // LUMINANCE_ALPHA_88
                               1,   // HILO_88
                               1,   // LUMINANCE_8
                               1,   // ALPHA_8
                               1,   // LUMINANCE_ALPHA_44
                               0.5, // LUMINANCE_4
                               0.5, // ALPHA_4
                               0.5, // ETC1_RGB8
                               1    // ETC1_A4
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
