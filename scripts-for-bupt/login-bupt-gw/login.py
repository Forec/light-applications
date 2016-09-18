#coding=utf-8

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

req = urllib2.Request(
    url = 'http://10.3.8.211',
    data = postdata
)

opener.open(req)