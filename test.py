from PIL import Image
from ctypes import *
import os


path = os.path.dirname(os.path.realpath(__file__))
DLL_NAME = "lib3dstex.dll"
DLL_FULL_NAME = os.path.join(path, DLL_NAME)
if not os.path.exists(DLL_FULL_NAME):
    DLL_FULL_NAME = os.path.join(os.getcwd(), DLL_NAME)
DLL = cdll.LoadLibrary(DLL_FULL_NAME)

FORMATS = {'RGBA8888': 0,
           'RGB888': 1,
           'RGBA_5551': 2,
           'RGB_565': 3,
           'RGBA_4444': 4,
           'LUMINANCE_ALPHA_88': 5,
           'HILO_88': 6,
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


def get_encoded_buf(image_name, fmt):
    os.system(f'ctr_TexturePackager32.exe -l "{image_name},format({fmt})" -nt -o tmp.ctpk')
    return open('tmp.ctpk', 'rb').read()[0x80:]


def get_same_ratio(buf1, buf2):
    s = 0
    for i, b in enumerate(buf1):
        s += (0xff - abs(b - buf2[i])) / 0xff
    return s / len(buf1)


def test(image_name, fmt):
    im = Image.open(image_name)
    w, h = im.size
    mode = im.mode
    raw = im.tobytes()

    encode_buf = get_encoded_buf(image_name, fmt)
    buf = decode(encode_buf, w, h, FORMATS[fmt])

    im = Image.new(mode, (w, h))
    im.frombytes(buf)
    im.save(f'{FORMATS[fmt]}_{fmt}.png')

    my_encode_buf = encode(raw, w, h, FORMATS[fmt])
    ratio = get_same_ratio(my_encode_buf, encode_buf * 100)
    print(f'*** {FORMATS[fmt]} {fmt} {ratio*100:.2f}% ***')
    open(f'{FORMATS[fmt]}_{fmt}.bin', 'wb').write(encode_buf)
    open(f'{FORMATS[fmt]}_{fmt}_my.bin', 'wb').write(my_encode_buf)

    im = Image.new(mode, (w, h))
    im.frombytes(decode(my_encode_buf, w, h, FORMATS[fmt]))
    im.save(f'{FORMATS[fmt]}_{fmt}_my.png')


test('rgba.tga', 'RGBA8888')
test('rgb.tga', 'RGB888')
test('rgb.tga', 'ETC1')
