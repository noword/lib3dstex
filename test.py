from PIL import Image
from lib3dstex import encode, decode, FORMATS
import os


def get_encoded_buf(image_name, fmt):
    os.system(f'ctr_TexturePackager32.exe -l "{image_name},format({fmt})" -nt -o tmp.ctpk')
    return open('tmp.ctpk', 'rb').read()[0x80:]


def get_same_ratio(buf1, buf2):
    s = 0
    for i, b in enumerate(buf1):
        s += (0xff - abs(b - buf2[i])) / 0xff
    return s / len(buf1)


def test(image_name, fmt, mode=None):
    im = Image.open(image_name)
    w, h = im.size
    if mode is None:
        mode = im.mode
    else:
        im = im.convert(mode)
    raw = im.tobytes()

    encode_buf = get_encoded_buf(image_name, fmt)
    decode_buf = decode(encode_buf, w, h, FORMATS[fmt])

    im = Image.new(mode, (w, h))
    im.frombytes(decode_buf)
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
test('rgba.tga', 'RGBA5551')
test('rgb.tga', 'RGB565')
test('rgba.tga', 'RGBA4444')
test('rgba.tga', 'LA88', 'LA')
test('rgb.tga', 'HL8')
test('rgb.tga', 'L8', 'L')
test('rgb.tga', 'A8', 'L')
test('rgba.tga', 'LA44', 'LA')
test('rgb.tga', 'ETC1')
test('rgba.tga', 'ETC1_A4')
