# last edit date: 2016/09/24
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

import os
import time
import requests
import argparse
import threading
import selenium
import browsermobproxy
from selenium.webdriver.common.desired_capabilities \
	import DesiredCapabilities

parser = argparse.ArgumentParser()
parser.add_argument('--url', type=str, default='')
parser.add_argument('--time', type=int, default=30)
parser.add_argument('--proxy', type=str, default="browsermob-proxy.bat")
parser.add_argument('--bin', type=str, default='')
args = parser.parse_args()
URL = args.url
WAIT = args.time
PROXY = args.proxy
BIN = args.bin

class Downloader(threading.Thread):
	def __init__(self, url, num, suffix):
		threading.Thread.__init__(self)
		self.setDaemon(False)
		self.url = url
		self.num = num
		self.suffix = suffix
	def run(self):
		if self.url == '':
			return
		data = requests.get(self.url, timeout=10)
		if data is None:
			return
		filename = self.num + '.' + self.suffix
		with open(filename, 'wb') as f:
			f.write(data.content)

class Simulator:
	def __init__(self, suffixs=['mp4', 'flv', 'avi', 'rmvb', 'rm']):
		self.suffixs = suffixs
		self.video_box = {}
		self.server = browsermobproxy.Server(PROXY)

	def simulate(self, video_page, wait):
		self.server.start()
		proxy = self.server.create_proxy()
		profile = selenium.webdriver.FirefoxProfile()
		profile.set_proxy(proxy.selenium_proxy())
		caps = DesiredCapabilities.FIREFOX
		caps["marionette"] = True
		if BIN != '':
			caps["binary"] = BIN
		driver = selenium.webdriver.Firefox(capabilities=caps, firefox_profile=profile)
		proxy.new_har("search")

		driver.get(video_page)
		time.sleep(wait)
		content = proxy.har
		self.server.stop()
		driver.quit()

		data = content['log']['entries']
		for record in range(data):
			url = record['request']['url']
			for suffix in self.suffixs:
				if url.find("."+suffix) != -1:
					if self.video_box.get(suffix) is None:
						self.video_box[suffix] = [url]
					else:
						self.video_box[suffix].append(url)
					break
		# download video, save to ./download
		if len(self.video_box) != 0:
			try:
				os.mkdir('download')
			except:
				pass
			os.chdir('download')
			for (suffix, urls) in self.video_box.items():
				for i in range(len(urls)):
					downloader = Downloader(urls[i], str(i), suffix)
					downloader.start()
		else:
			print("No videos found...")

if __name__ == '__main__':
	simulator = Simulator()
	simulator.simulate(URL, WAIT)