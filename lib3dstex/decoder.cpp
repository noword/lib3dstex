#include <stdint.h>
#include "rg_etc1.h"
#include "tex3ds.h"
#include "utils.h"

void decode_rgba8(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block32((uint32_t *)inbuf, width, height, (uint32_t *)outbuf);
    swap32s((uint32_t *)outbuf, width * height);
}

void decode_rgb8(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block24(inbuf, width, height, outbuf);
    swap24s(outbuf, width * height);
}

void decode_rgba5551(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    size_t    size = width * height;
    uint16_t *tmp  = new uint16_t[size];
    decode_block16((uint16_t *)inbuf, width, height, tmp);
    for (int i = 0; i < size; i++)
    {
        uint16_t t = tmp[i];
        *outbuf++ = (t >> 11) << 3;         //R
        *outbuf++ = ((t >> 6) & 0x1f) << 3; //G
        *outbuf++ = ((t >> 1) & 0x1f) << 3; //B
        *outbuf++ = (t & 1) * 0xff;         //A
    }
    delete[]tmp;
}

void decode_rgb565(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    size_t    size = width * height;
    uint16_t *tmp  = new uint16_t[size];
    decode_block16((uint16_t *)inbuf, width, height, tmp);
    for (int i = 0; i < size; i++)
    {
        uint16_t t = tmp[i];
        *outbuf++ = (t >> 11) << 3;         //R
        *outbuf++ = ((t >> 5) & 0x3f) << 2; //G
        *outbuf++ = (t & 0x1f) << 3;        //B
    }
    delete[]tmp;
}

void decode_rgba4444(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    size_t    size = width * height;
    uint16_t *tmp  = new uint16_t[size];
    decode_block16((uint16_t *)inbuf, width, height, tmp);
    for (int i = 0; i < size; i++)
    {
        uint16_t t = tmp[i];
        *outbuf++ = (t >> 12) << 4;        //R
        *outbuf++ = ((t >> 8) & 0xf) << 4; //G
        *outbuf++ = ((t >> 4) & 0xf) << 4; //B
        *outbuf++ = (t & 0xf) << 4;        //A
    }
    delete[]tmp;
}

void decode_la88(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block16((uint16_t *)inbuf, width, height, (uint16_t *)outbuf);
    swap16s((uint16_t *)outbuf, width * height);
}

void decode_hilo88(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    size_t    size = width * height;
    uint16_t *tmp  = new uint16_t[size];
    decode_block16((uint16_t *)inbuf, width, height, tmp);
    uint8_t *p = (uint8_t *)tmp;
    for (int i = 0; i < size; i++)
    {
        *outbuf++ = p[1]; //R
        *outbuf++ = p[0]; //G
        *outbuf++ = 0;    //B
        p        += 2;
    }
    delete[]tmp;
}

void decode_l8(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block8(inbuf, width, height, outbuf);
}

void decode_a8(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_block8(inbuf, width, height, outbuf);
}

void decode_la44(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    size_t   size = width * height;
    uint8_t *tmp  = new uint8_t[size];
    decode_block8(inbuf, width, height, tmp);
    for (int i = 0; i < size; i++)
    {
        *outbuf++ = tmp[i] & 0xf0;
        *outbuf++ = (tmp[i] & 0xf) << 4;
    }
    delete[]tmp;
}

void decode_l4(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    uint8_t *tmp = new uint8_t[width * height];
    uint8_t *p   = tmp;
    for (int i = 0; i < width * height / 2; i++)
    {
        *p++ = (inbuf[i] & 0xf) << 4;
        *p++ = inbuf[i] & 0xf0;
    }
    decode_block8(tmp, width, height, outbuf);
    delete[]tmp;
}

void decode_a4(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    decode_l4(inbuf, width, height, outbuf);
}

void decode_etc1(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
{
    uint64_t *in64 = (uint64_t *)inbuf;

    for (int ty = 0; ty < height; ty += 8)
    {
        for (int tx = 0; tx < width; tx += 8)
        {
            uint32_t pixels[64];
            for (int i = 0; i < 4; i++)
            {
                uint64_t tmp = bswap64(*in64++);
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

void decode_etc1a4(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf)
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

                uint64_t tmp = bswap64(*in64++);
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
                                        decode_rgba5551,
                                        decode_rgb565,
                                        decode_rgba4444,
                                        decode_la88,
                                        decode_hilo88,
                                        decode_l8,
                                        decode_a8,
                                        decode_la44,
                                        decode_l4,
                                        decode_a4,
                                        decode_etc1,
                                        decode_etc1a4,
};

const int DECODE_RATIO[] = { 4, // RGBA_8888
                             3, // RGB_888
                             4, // RGBA_5551
                             3, // RGB_565
                             4, // RGBA_4444,
                             2, // LUMINANCE_ALPHA_88
                             3, // HILO_88,
                             1, // LUMINANCE_8
                             1, // ALPHA_8,
                             2, // LUMINANCE_ALPHA_44
                             1, // LUMINANCE_4
                             1, // ALPHA_4,
                             3, // ETC1_RGB8,
                             4  // ETC1_A4,
};

EXPORT size_t get_decode_size(size_t width, size_t height, TEXTURE_FORMAT format)
{
    return format < FORMAT_NUM ? width * height * DECODE_RATIO[format]:0;
}

EXPORT void decode(const uint8_t *inbuf, size_t width, size_t height, TEXTURE_FORMAT format, uint8_t *outbuf)
{
    if (format < FORMAT_NUM)
    {
        return DECODE_FUNCTIONS[format](inbuf, width, height, outbuf);
    }
}
