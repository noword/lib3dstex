from PIL import Image
from lib3dstex import encode, decode, TEXTURE_3DS_FORMATS
import os


def get_encoded_buf_offical(im_name, fmt):
    os.system(f'ctr_TexturePackager32.exe -l "{im_name},format({fmt})" -nt -o tmp.ctpk')
    return open('tmp.ctpk', 'rb').read()[0x80:]


def get_encoded_buf_tex3ds(im_name, fmt):
    os.system(f'tex3ds -f {fmt} -z none -r -o tmp.bin {im_name}')
    return open('tmp.bin', 'rb').read()[4:]


get_encoded_buf = get_encoded_buf_offical


def get_same_ratio(buf1, buf2):
    s = 0
    for i, b in enumerate(buf1):
        s += (0xFF - abs(b - buf2[i])) / 0xFF
    return s / len(buf1)


def test(im_name, fmt):
    index, mode = TEXTURE_3DS_FORMATS[fmt]
    im = Image.open(im_name)
    w, h = im.size
    if mode != im.mode:
        im = im.convert(mode)
    raw = im.tobytes()

    encode_buf = get_encoded_buf(im_name, fmt)
    decode_buf = decode(encode_buf, w, h, index)

    im = Image.new(mode, (w, h))
    im.frombytes(decode_buf)
    im.save(f'{index}_{fmt}.png')

    my_encode_buf = encode(raw, w, h, index)
    ratio = get_same_ratio(my_encode_buf, encode_buf * 100)
    print(f'*** {index} {fmt} {ratio*100:.2f}% ***')
    open(f'{index}_{fmt}.bin', 'wb').write(encode_buf)
    open(f'{index}_{fmt}_my.bin', 'wb').write(my_encode_buf)

    im = Image.new(mode, (w, h))
    im.frombytes(decode(my_encode_buf, w, h, index))
    im.save(f'{index}_{fmt}_my.png')


test('rgba.tga', 'RGBA8888')
test('rgb.tga', 'RGB888')
test('rgba.tga', 'RGBA5551')
test('rgb.tga', 'RGB565')
test('rgba.tga', 'RGBA4444')
test('rgba.tga', 'LA88')
test('rgb.tga', 'HL8')
test('rgb.tga', 'L8')
test('rgb.tga', 'A8')
test('rgba.tga', 'LA44')
test('rgb.tga', 'L4')
test('rgb.tga', 'A4')
test('rgb.tga', 'ETC1')
test('rgba.tga', 'ETC1_A4')
