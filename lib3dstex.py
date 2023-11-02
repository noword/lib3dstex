from ctypes import *
import os
from PIL import Image

__path = os.path.dirname(os.path.realpath(__file__))
DLL_NAME = 'lib3dstex.dll'
DLL_FULL_NAME = os.path.join(__path, DLL_NAME)
if not os.path.exists(DLL_FULL_NAME):
    DLL_FULL_NAME = os.path.join(os.getcwd(), DLL_NAME)
DLL = cdll.LoadLibrary(DLL_FULL_NAME)

FORMATS = {'RGBA8888': (0, 'RGBA'),
           'RGB888': (1, 'RGB'),
           'RGBA5551': (2, 'RGBA'),
           'RGB565': (3, 'RGB'),
           'RGBA4444': (4, 'RGBA'),
           'LA88': (5, 'LA'),
           'HL8': (6, 'RGB'),
           'L8': (7, 'L'),
           'A8': (8, 'L'),
           'LA44': (9, 'LA'),
           'L4': (10, 'L'),
           'A4': (11, 'L'),
           'ETC1': (12, 'RGB'),
           'ETC1_A4': (13, 'RGBA')}


def __codec(size_func: CFUNCTYPE, codec_func: CFUNCTYPE, buf: bytes, width: int, height: int, fmt: str) -> bytes:
    size = size_func(width, height, fmt)
    out = create_string_buffer(size)
    codec_func(buf, width, height, fmt, out)
    return out.raw


def encode(buf: bytes, width: int, height: int, fmt: str) -> bytes:
    return __codec(DLL.get_encode_size, DLL.encode, buf, width, height, fmt)


def decode(buf: bytes, width: int, height: int, fmt: str) -> bytes:
    return __codec(DLL.get_decode_size, DLL.decode, buf, width, height, fmt)


def bin2image(buf: bytes, width: int, height: int, fmt: str) -> Image:
    im = Image.new(FORMATS[fmt][1], (width, height))
    im.frombytes(decode(buf, width, height, fmt))
    return im


def image2bin(im: Image, fmt: str) -> bytes:
    index, mode = FORMATS[fmt]
    width, height = im.size
    return encode(im.convert(mode).tobytes(), widht, height, index)
