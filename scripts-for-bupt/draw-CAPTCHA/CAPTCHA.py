#coding = utf-8

# last edit date: 2016/09/23
# author: Forec
# LICENSE
# Copyright (c) 2015-2017, Forec <forec@bupt.edu.cn>

# Permission to use, copy, modify, and/or distribute this code for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.

# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

import random
from PIL import Image, ImageDraw, ImageFont, ImageFilter
import sys

if len(sys.argv) > 4:
    print "Usage: python2 CAPTCHA.py <numbers_in_CAPTCHA> <width>, default is 4, 400"
    sys.exit(0)
elif len(sys.argv) == 1:
    numbers = 4
    width = 400
    height = 200
else:
    temp = []
    for arg in sys.argv[1:]:
        try:
            temp.append(int(arg))
        except:
            print "Invalid param..."
            sys.exit(0)
    numbers = temp[0]
    width = temp[1]
    height = width//2

def random_col():
    return (random.randint(50,200),random.randint(50,200),random.randint(50,200))

def make( numbers, width = 400, height = 200):
    strs = ''.join(random.sample('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789', numbers))
    im = Image.new( 'RGB', (width, height ), (255,255,255))
    draw = ImageDraw.Draw(im)
    font = ImageFont.truetype('verdana.ttf',width//numbers)
    font_width , font_height = font.getsize(strs)
    strs_len = len(strs)
    x = (width - font_width) // 2
    y = (height - font_height ) //2
    total_dex = 0
    for i in strs:
        draw.text((x,y), i, random_col(), font)
        temp = random.randint(-23,23)
        total_dex += temp
        im = im.rotate(temp)
        draw = ImageDraw.Draw(im)
        x += font_width/strs_len
    im = im.rotate(-total_dex)
    draw = ImageDraw.Draw(im)
    draw.line(
        [(random.randint(0,width//numbers),
        random.randint(0,height//numbers)
        ),
        (random.randint(width//numbers*(numbers-1),width),
        random.randint(height//numbers*(numbers-1),height)
        )],
        fill = random_col(),
        width = numbers+1)
    draw.line(
        [(random.randint(0,width//numbers),
            random.randint(height//numbers*(numbers-1),height)
        ),
        (random.randint(width//(numbers-1)*(numbers-2),width),
        random.randint(0,height//(numbers-1))
        )],
        fill = random_col(),
        width = numbers+1)
    draw.line(
        [(random.randint(width//4*3,width),
            random.randint(height//4*3,height)
        ),
        (random.randint(width//3*2,width),
        random.randint(0,height//3)
        )],
        fill = random_col(),
        width = numbers + 1)
    for x in range(width):
        for y in range(height):
            col = im.getpixel((x,y))
            if col == (255,255,255) or col == (0,0,0):
                draw.point((x,y), fill = random_col())
    im = im.filter(ImageFilter.BLUR)
    im.save('out.jpg')
    
if __name__ == '__main__':
    make(numbers, width, height)