from ctypes import *
import os

__path = os.path.dirname(os.path.realpath(__file__))
DLL_NAME = 'lib3dstex.dll'
DLL_FULL_NAME = os.path.join(__path, DLL_NAME)
if not os.path.exists(DLL_FULL_NAME):
    DLL_FULL_NAME = os.path.join(os.getcwd(), DLL_NAME)
DLL = cdll.LoadLibrary(DLL_FULL_NAME)

FORMATS = {'RGBA8888': 0,
           'RGB888': 1,
           'RGBA5551': 2,
           'RGB565': 3,
           'RGBA4444': 4,
           'LA88': 5,
           'HL8': 6,
           'LUMINANCE_8': 7,
           'ALPHA_8': 8,
           'LUMINANCE_ALPHA_44': 9,
           'LUMINANCE_4': 10,
           'ALPHA_4': 11,
           'ETC1': 12,
           'ETC1_A4': 13, }


def __codec(size_func, codec_func, buf, width, height, fmt):
    size = size_func(width, height, fmt)
    out = create_string_buffer(size)
    codec_func(buf, width, height, fmt, out)
    return out.raw


def encode(buf, width, height, fmt):
    return __codec(DLL.get_encode_size, DLL.encode, buf, width, height, fmt)


def decode(buf, width, height, fmt):
    return __codec(DLL.get_decode_size, DLL.decode, buf, width, height, fmt)
