# -*- coding: utf-8-unix -*-

# Extracts 2 bitmap images (*.pbm) from a 4-colors image.

import os
import sys

from PIL import Image

path = sys.argv[1]
file, ext = os.path.splitext(path)

with Image.open(path) as im:
    im2 = im.convert(mode='P', colors=4)
    # im2.save(file + '_2bpp.png')
    a = im2.point(lambda x: x & 1, mode='1')
    b = im2.point(lambda x: x & 2, mode='1')
    a.save(file + '_1.pbm')
    b.save(file + '_2.pbm')
    # a = a.convert('L')
    # b = b.convert('L')
    # c = Image.new('L', im2.size)
    # Image.merge('RGB', [a, b, c]).show()

