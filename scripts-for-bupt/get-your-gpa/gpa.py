#coding=utf8

# last edit date: 2016/09/19
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

import urllib, urllib2, cookielib, re, sys, os
import pytesseract
from PIL import Image
from bs4 import BeautifulSoup

if len(sys.argv) != 3:
	print "Usage: python2 gpa.py <id> <pwd>"
	sys.exit(0)
else:
	username = sys.argv[1]
	password = sys.argv[2]

cookie = cookielib.CookieJar()
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookie))

print "Connecting URP..."
tryTime = 0

while True:
	tryTime += 1
	if tryTime > 5:
		os.remove("check.jpeg")
		print "Cannot connect URP!"
		sys.exit(0)
	headers = {
		'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:48.0) Gecko/20100101 Firefox/48.0',
		'Host': 'jwxt.bupt.edu.cn',
		'Referer': 'http://jwxt.bupt.edu.cn/',
		'Connection': 'keep-alive',
		'DNT' : '1',
		'Cookie': 'JSESSIONID=abc1F_5Ygjp5DcGVBa8Cv'
	}
	req = urllib2.Request(
		url = 'http://jwxt.bupt.edu.cn/validateCodeAction.do?random=',
		headers = headers
	)
	result = opener.open(req)
	with open("check.jpeg", "wb") as f:
		f.write(result.read())
	# Get Optical Character Recognition
	checkInfo = str(pytesseract.image_to_string(Image.open('check.jpeg')))

	postdata=urllib.urlencode({
		'type' : 'sso',
	    'zjh'  : username,
	    'mm'   : password,
	    'v_yzm': checkInfo
	})

	del headers['DNT']
	headers['Upgrade-Insecure-Requests'] = '1'
	req = urllib2.Request(    
	    url = 'http://jwxt.bupt.edu.cn/jwLoginAction.do', 
	    data = postdata,
	    headers = headers
	)
	result = opener.open(req)
	bt = BeautifulSoup(result.read(), "html.parser",from_encoding="gbk")
	if bt.title.string[:3] != "URP":
		break

os.remove("check.jpeg")

headers['Referer'] = 'http://jwxt.bupt.edu.cn/gradeLnAllAction.do?type=ln&oper=qb'
req = urllib2.Request(
	url = 'http://jwxt.bupt.edu.cn/gradeLnAllAction.do?type=ln&oper=sxinfo&lnsxdm=001',
	headers = headers)

bt = BeautifulSoup(opener.open(req).read(), "html.parser",from_encoding="gbk")
tables = bt.find_all('tr', class_ ='odd')

score = 0.0; count = 0.0
for table in tables:
	temp = BeautifulSoup(table.encode('gbk'), "html.parser", from_encoding='gbk')
	items = temp.find_all('td', align = 'center')
	if items[5].encode('gbk')[38:-18] == "\xc8\xce\xd1\xa1\r":	# 任选
		continue
	if items[6].encode('gbk')[38:-18] == "\xc3\xe2":	# 免修
		continue
	print str(int(items[0].encode('gbk')[37:-19])) + "\t",
	print str(float(items[4].encode('gbk')[38:-18])) + "\t",
	print str(int(items[6].encode('gbk')[38:-18])) + "\t",
	print items[2].encode('gbk')[38:-18].decode('gbk')
	factor = float(items[4].encode('gbk')[38:-18])
	score += (float(items[6].encode('gbk')[38:-18]) * factor)
	count += factor

print ("GPA is : %.2f" % (float(score)/count))

headers['DNT'] = '1'
headers['Referer'] = 'http://jwxt.bupt.edu.cn/menu/s_top.jsp'
postdata = urllib.urlencode({
	'loginType' : 'jwLogin'	
})
req = urllib2.Request(    
    url = 'http://jwxt.bupt.edu.cn/logout.do', 
    data = postdata,
    headers = headers
)
opener.open(req)