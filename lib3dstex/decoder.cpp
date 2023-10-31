#include <stdint.h>
#include "rg_etc1.h"
#include "tex3ds.h"
#include "utils.h"

void decode_rgba8(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block32((uint32_t *)inbuf, width, height, (uint32_t *)outbuf);
    swap32s((uint32_t *)outbuf, width * height);
}

void decode_rgb8(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block24(inbuf, width, height, outbuf);
    swap24s(outbuf, width * height);
}

void decode_etc1(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    uint64_t *in64 = (uint64_t *)inbuf;

    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            uint32_t pixels[64];
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

                uint8_t *out = (uint8_t *)(outbuf + ((ty + y) * width + tx + x) * 3);
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
    uint64_t *in64 = (uint64_t *)inbuf;
    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            uint32_t pixels[64];
            uint32_t alpha[64];

            for (int i = 0; i < 4; i++)
            {
                uint64_t a = *in64++;
                for (int j = 0; j < 16; j++)
                {
                    alpha[i * 16 + ETC1_ALPHA_ORDER[j]] = (a & 0xf) << 28;
                    a >>= 4;
                }

                uint64_t tmp = bswap_64(*in64++);
                rg_etc1::unpack_etc1_block(&tmp, pixels + i * 16, false);
            }

            for (int i = 0; i < 64; i++)
            {
                size_t    order = ETC1_TILE_ORDER[i];
                size_t    x     = order & 7;
                size_t    y     = order >> 3;
                uint32_t *out   = (uint32_t *)(outbuf + ((ty + y) * width + tx + x) * 4);
                *out = (pixels[i] & 0xffffff) | alpha[i];
            }
        }
    }
}

const CODEC_FUNC DECODE_FUNCTIONS[] = { decode_rgba8,
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
                                        decode_etc1a4,
};

const int DECODE_RATIO[] = { 4, //RGBA_8888
                             3, //RGB_888
                             4, //RGBA_5551
                             3, //RGB_565
                             4, //RGBA_4444,
                             2, //LUMINANCE_ALPHA_88
                             1, //HILO_88,
                             1, //LUMINANCE_8
                             1, //ALPHA_8,
                             1, //LUMINANCE_ALPHA_44
                             1, //LUMINANCE_4
                             1, //ALPHA_4,
                             3, //ETC1_RGB8,
                             4  //ETC1_A4,
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
