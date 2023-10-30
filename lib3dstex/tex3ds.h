#pragma once

#ifdef  __cplusplus
#define EXPORT    extern "C" __declspec(dllexport)
#else
#define EXPORT    __declspec(dllexport)
#endif

enum TEXTURE_FORMAT
{
    RGBA_8888 = 0,
    RGB_888 = 1,
    RGBA_5551 = 2,
    RGB_565 = 3,
    RGBA_4444 = 4,
    LUMINANCE_ALPHA_88 = 5,
    HILO_88 = 6,
    LUMINANCE_8 = 7,
    ALPHA_8 = 8,
    LUMINANCE_ALPHA_44 = 9,
    LUMINANCE_4 = 10,
    ALPHA_4 = 11,
    ETC1_RGB8 = 12,
    ETC1_A4 = 13,
    FORMAT_NUM,
};

const int TILE_ORDER[] = { 0,   1,  8,  9,  2,  3, 10, 11,
                           16, 17, 24, 25, 18, 19, 26, 27,
                           4,   5, 12, 13,  6,  7, 14, 15,
                           20, 21, 28, 29, 22, 23, 30, 31,
                           32, 33, 40, 41, 34, 35, 42, 43,
                           48, 49, 56, 57, 50, 51, 58, 59,
                           36, 37, 44, 45, 38, 39, 46, 47,
                           52, 53, 60, 61, 54, 55, 62, 63 };

const int ETC1_TILE_ORDER[] = { 0,   1,  2,  3,  8,  9, 10, 11,
                                16, 17, 18, 19, 24, 25, 26, 27,
                                4,   5,  6,  7, 12, 13, 14, 15,
                                20, 21, 22, 23, 28, 29, 30, 31,
                                32, 33, 34, 35, 40, 41, 42, 43,
                                48, 49, 50, 51, 56, 57, 58, 59,
                                36, 37, 38, 39, 44, 45, 46, 47,
                                52, 53, 54, 55, 60, 61, 62, 63, };

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
                               1,   // ETC1_RGB8
                               0.5  // ETC1_A4
};

typedef void (*CODEC_FUNC)(uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf);
