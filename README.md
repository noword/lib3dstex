a library for decoding and encoding 3DS textures
# Supported formats
```c
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
    ETC1_RGB8 = 12,
    ETC1_A4 = 13,
}
```

# C API
```c
EXPORT size_t get_encode_size(size_t width, size_t height, TEXTURE_FORMAT format);
EXPORT size_t get_decode_size(size_t width, size_t height, TEXTURE_FORMAT format);
EXPORT void encode(const uint8_t *inbuf, size_t width, size_t height, TEXTURE_FORMAT format, uint8_t *outbuf);
EXPORT void decode(const uint8_t *inbuf, size_t width, size_t height, TEXTURE_FORMAT format, uint8_t *outbuf);
```

# python API
```python
def encode(buf: bytes, width: int, height: int, fmt: int) -> bytes:
def decode(buf: bytes, width: int, height: int, fmt: int) -> bytes:
def bin2image(buf: bytes, width: int, height: int, fmt: str) -> Image:
def image2bin(im: Image, fmt: str) -> bytes:
```
Image is the PIL Image object.

Please refer to lib3dstex.py for more detailed information.