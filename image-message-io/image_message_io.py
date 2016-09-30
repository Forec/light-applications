# last edit date: 2016/09/30
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

class ImageMessageWriter(object):
	def __init__(self):
		self.image = None
		self.encodedImage = None
		self.image7bits = None
		self.encodeString = ""

	def loadImage(self, image_or_path):
		if isinstance(image_or_path, Image.Image):
			self.image = image_or_path
		elif isinstance(image_or_path, str):
			self.image = Image.open(image_or_path)
		else:
			raise TypeError("Invalid parameter.")
		self.image7bits = self.__generate_image_7bits_()

	def encode(self, message):
		if self.image is None or self.image7bits is None:
			raise Exception("No image loaded.")
		if message is None:
			raise Exception("No message assigned.")
		self.encodeString = message
		encodedMessages = ''.join(map(self.__chr2bin_, bytearray(message, 'utf-8')))
		if len(encodedMessages) + 32 > len(self.image.getdata()) * 8:
			raise Exception("Error: Can't encode more than " + len(evenImage.getdata()) * 8 - 32+ " bits in current image.")
		encodedPixels = []; lastIndex = 0
		messageLength = ''.join(self.__chr2bin_(len(encodedMessages)))
		messageLength = '0' * (32 - len(messageLength)) + messageLength
		for index, (r, g, b, t) in enumerate(list(self.image7bits.getdata())):
			if index < 8:
				encodedPixels.append((r + int(messageLength[index * 4]),
									  g + int(messageLength[index*4+1]),
									  b + int(messageLength[index*4+2]),
									  t + int(messageLength[index*4+3])))
				continue
			if index * 4 - 32 < len(encodedMessages):
				encodedPixels.append((r + int(encodedMessages[index * 4 - 32]), 
									  g + int(encodedMessages[index*4+1 - 32]),
									  b + int(encodedMessages[index*4+2 - 32]),
									  t + int(encodedMessages[index*4+3 - 32])))
			else:
				lastIndex = index
				break
		for index, (r, g, b, t) in enumerate(list(self.image.getdata())):
			if index >= lastIndex:
				encodedPixels.append((r, g, b, t))
		self.encodedImage = Image.new(self.image7bits.mode, self.image7bits.size)
		self.encodedImage.putdata(encodedPixels)

	def saveImage(self, filepath):
		if self.encodedImage is None:
			raise Warning("You didn't encode any message into this image")
		else:
			self.encodedImage.save(filepath)

	def __generate_image_7bits_(self):
		pixels = list(self.image.getdata())
		pixels7bits = [(r>>1<<1, g>>1<<1, b>>1<<1, t>>1<<1) for [r,g,b,t] in pixels]
		image7bits = Image.new(self.image.mode, self.image.size)
		image7bits.putdata(pixels7bits)
		return image7bits

	@staticmethod
	def __chr2bin_(chr):
		ans = "0" * ( 10 - (len( bin(chr) )) );
		ans += bin(chr).replace('0b','')
		return ans

class ImageMessageReader(object):
	def __init__(self):
		self.image = None
		self.decodeMessage = None

	def loadImage(self, image_or_path):
		if isinstance(image_or_path, Image.Image):
			self.image = image_or_path
		elif isinstance(image_or_path, str):
			self.image = Image.open(image_or_path)
		else:
			raise TypeError("Invalid parameter.")

	def decode(self):
		pixels = list(self.image.getdata())
		bins = ''.join([str(int(r>>1<<1!=r))+str(int(g>>1<<1!=g))+str(int(b>>1<<1!=b))+str(int(t>>1<<1!=t)) for (r,g,b,t) in pixels])
		length = int(bins[:32], 2)
		self.decodeMessage = self.__utf2str_(bins[32:length + 32])
		return self.decodeMessage

	def getMessage(self):
		if self.decodeMessage is None:
			raise Exception("Message is None. Check whether you have decoded.")
		return self.decodeMessage

	@staticmethod
	def __utf2str_(utf):
		i = 0; ans = []
		_utf16rec8_ = lambda x, i: x[2:8] + (_utf16rec8_(x[8:], i-1) if i > 1 else '') if x else ''
		_utf16rec16_ = lambda x, i: x[i+1:8] + _utf16rec8_(x[8:], i-1)
		while i + 1 < len(utf):
			chartype = utf[i:].index('0')
			length = chartype * 8 if chartype else 8
			ans.append(chr(int(_utf16rec16_(utf[i:i+length], chartype), 2)))
			i += length
		return ''.join(ans)

if __name__ == '__main__':
	imgWriter = ImageMessageWriter()
	imgWriter.loadImage('test.png')
	imgWriter.encode('This is a test case for 中文 condition.')
	imgWriter.saveImage('testEncoded.png')
	imgReader = ImageMessageReader()
	imgReader.loadImage('testEncoded.png')
	print(imgReader.decode())