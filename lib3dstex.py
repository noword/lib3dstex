from ctypes import *
import os
from PIL import Image
from typing import Union

__path = os.path.dirname(os.path.realpath(__file__))
DLL_NAME = "lib3dstex.dll"
DLL_FULL_NAME = os.path.join(__path, DLL_NAME)
if not os.path.exists(DLL_FULL_NAME):
    DLL_FULL_NAME = os.path.join(os.getcwd(), DLL_NAME)
DLL = cdll.LoadLibrary(DLL_FULL_NAME)

TEXTURE_3DS_FORMATS = {
    "RGBA8888": (0, "RGBA"),
    "RGB888": (1, "RGB"),
    "RGBA5551": (2, "RGBA"),
    "RGB565": (3, "RGB"),
    "RGBA4444": (4, "RGBA"),
    "LA88": (5, "LA"),
    "HL8": (6, "RGB"),
    "L8": (7, "L"),
    "A8": (8, "L"),
    "LA44": (9, "LA"),
    "L4": (10, "L"),
    "A4": (11, "L"),
    "ETC1": (12, "RGB"),
    "ETC1_A4": (13, "RGBA"),
}


def _get_format_index(fmt):
    if isinstance(fmt, str):
        return TEXTURE_3DS_FORMATS[fmt][0]
    return fmt


def get_encode_size(width: int, height: int, fmt: Union[int, str]) -> int:
    return DLL.get_encode_size(width, height, _get_format_index(fmt))


def get_decode_size(width: int, height: int, fmt: Union[int, str]) -> int:
    return DLL.get_decode_size(width, height, _get_format_index(fmt))


def encode(buf: bytes, width: int, height: int, fmt: Union[int, str]) -> bytes:
    fmt = _get_format_index(fmt)
    size = get_encode_size(width, height, fmt)
    out = create_string_buffer(size)
    DLL.encode(buf, width, height, fmt, out)
    return out.raw


def decode(buf: bytes, width: int, height: int, fmt: Union[int, str]) -> bytes:
    fmt = _get_format_index(fmt)
    size = get_decode_size(width, height, fmt)
    out = create_string_buffer(size)
    DLL.decode(buf, width, height, fmt, out)
    return out.raw


def bin2image(buf: bytes, width: int, height: int, fmt: str) -> Image:
    index, mode = TEXTURE_3DS_FORMATS[fmt]
    im = Image.new(mode, (width, height))
    im.frombytes(decode(buf, width, height, index))
    return im


def image2bin(im: Image, fmt: str) -> bytes:
    index, mode = TEXTURE_3DS_FORMATS[fmt]
    width, height = im.size
    return encode(im.convert(mode).tobytes(), width, height, index)
