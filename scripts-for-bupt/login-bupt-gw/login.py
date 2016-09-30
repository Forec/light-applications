#coding=utf-8

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

import time
import sys
import urllib
import urllib2
import cookielib
  
if len(sys.argv) != 3:
	print "Usage: python2 login.py <id> <pwd>"
	sys.exit(0)
else:
	username = sys.argv[1]
	password = sys.argv[2]

cookie = cookielib.CookieJar()
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookie))

postdata=urllib.urlencode({
    'DDDDD': username,
    'upass': password,
    'savePWD':'0',
    '0MKKey':''
})

time.sleep(5)

req = urllib2.Request(
    url = 'http://10.4.1.2',
    data = postdata
)
opener.open(req)

time.sleep(2)

req = urllib2.Request(
    url = 'http://10.3.8.211',
    data = postdata
)
opener.open(req)