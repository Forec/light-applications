# last edit date: 2016/09/22
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

from PIL import Image
import numpy as np
import cv2
import os
import argparse

parser = argparse.ArgumentParser()

parser.add_argument('--file')
parser.add_argument('--frame', type=int, default=5)
parser.add_argument('--color', type=bool, default=False)
parser.add_argument('--interval', type=int, default = 24)
parser.add_argument('-o', '--output',type=str, default="output")
parser.add_argument('--width', type=int, default=48)
parser.add_argument('--height', type=int, default=40)

args = parser.parse_args()

VEDIO = args.file
FRAME = args.frame
INTERVAL = args.interval
WIDTH = args.width
HEIGHT = args.height
OUTPUT = args.output
COLORFUL = args.color

ascii_char = list("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'.       ")

class Video2TXT:
	@staticmethod
	def get_char(r, g,b, alpha=256):
		if alpha == 0:
			return ' '
		length = len(ascii_char)
		gray = int(0.2126 * r + 0.7152 * g + 0.0722 * b)
		unit = (256.0+1)/length
		return ascii_char[int(gray/unit)]
	@staticmethod
	def get_gray(gray):
		if gray < 128:
			return '#'
		return ' '
	
	@staticmethod
	def writeVideoIntoTxt(vediofile, frames = 1, interval=1,output="output"):
		cap = cv2.VideoCapture(vediofile)
		try:
			os.mkdir(output)
		except:
			pass
		os.chdir(output)
		curframe = 0
		while (cap.isOpened()):
			ret, frame = cap.read()
			curframe += 1
			print curframe
			txt =""
			for i in range(interval):
				cap.read()
			im = Image.fromarray(frame)
			im.resize((WIDTH, HEIGHT), Image.ANTIALIAS)
			if COLORFUL:
				im = im.convert('RGB')
			else:
				im = im.convert('L')
				pix = im.load()
			for i in range(HEIGHT):
				for j in range(WIDTH):
					if COLORFUL:
						txt += Video2TXT.get_char(*im.getpixel((j,i)))		
					else:
						txt += Video2TXT.get_gray(pix[i, j])
				txt += '\n'
			with open("frame-"+str(curframe)+".txt", 'w') as f:
				f.write(txt)
			if curframe > frames:
				break

if __name__ == '__main__':
	if VEDIO is not None:
		Video2TXT.writeVideoIntoTxt(VEDIO, FRAME, INTERVAL,OUTPUT)
	print "done"
