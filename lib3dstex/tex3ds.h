#pragma once

#ifdef STATIC_LIB
    #define EXPORT
#else
    #ifdef  __cplusplus
        #define EXPORT    extern "C" __declspec(dllexport)
    #else
        #define EXPORT    __declspec(dllexport)
    #endif
#endif

enum TEXTURE_FORMAT
{
    RGBA_8888 = 0,          // RGBA 8888
    RGB_888 = 1,            // RGB 888
    RGBA_5551 = 2,          // RGBA 5551
    RGB_565 = 3,            // RGB 565
    RGBA_4444 = 4,          // RGBA 4444
    LUMINANCE_ALPHA_88 = 5, // a 8-bit luminance component and a 8-bit alpha value
    HILO_88 = 6,            // R and G components
    LUMINANCE_8 = 7,        // a 8-bit luminance component
    ALPHA_8 = 8,            // A8
    LUMINANCE_ALPHA_44 = 9, // a 4-bit luminance component and a 4-bit alpha value
    LUMINANCE_4 = 10,       // a 4-bit luminance component
    ALPHA_4 = 11,           // A4
    ETC1_RGB8 = 12,         // ETC1 format
    ETC1_A4 = 13,           // ETC1 format with alpha value.
    FORMAT_NUM,
};

const int ETC1_TILE_ORDER[] = { 0,   1,  2,  3,  8,  9, 10, 11, 16, 17, 18, 19, 24, 25, 26, 27,
                                4,   5,  6,  7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31,
                                32, 33, 34, 35, 40, 41, 42, 43, 48, 49, 50, 51, 56, 57, 58, 59,
                                36, 37, 38, 39, 44, 45, 46, 47, 52, 53, 54, 55, 60, 61, 62, 63 };

const int ETC1_ALPHA_ORDER[] = { 0,   4,  8, 12,  1,  5,  9, 13,  2,  6, 10, 14,  3,  7, 11, 15,
                                 16, 20, 24, 28, 17, 21, 25, 29, 18, 22, 26, 30, 19, 23, 27, 31,
                                 32, 36, 40, 44, 33, 37, 41, 45, 34, 38, 42, 46, 35, 39, 43, 47,
                                 48, 52, 56, 60, 49, 53, 57, 61, 50, 54, 58, 62, 51, 55, 59, 63 };

typedef void (*CODEC_FUNC)(const uint8_t *inbuf, size_t width, size_t height, uint8_t *outbuf);

EXPORT size_t get_encode_size(size_t width, size_t height, TEXTURE_FORMAT format);
EXPORT size_t get_decode_size(size_t width, size_t height, TEXTURE_FORMAT format);
EXPORT void encode(const uint8_t *inbuf, size_t width, size_t height, TEXTURE_FORMAT format, uint8_t *outbuf);
EXPORT void decode(const uint8_t *inbuf, size_t width, size_t height, TEXTURE_FORMAT format, uint8_t *outbuf);
