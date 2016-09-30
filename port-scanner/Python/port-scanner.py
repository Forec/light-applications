# last edit date: 2016/09/25
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

import sys
import threading
import time
from socket import *

class Scanner(threading.Thread):
    def __init__(self, port):
        threading.Thread.__init__(self)
        self.port = port
    def run(self):
        sock = socket(AF_INET, SOCK_STREAM)
        sock.settimeout(8)
        result = sock.connect_ex((target_ip, self.port))
        if result == 0:
            lock.acquire()
            print("Port Open:", self.port)
            lock.release()

if __name__=='__main__':
    host = sys.argv[1]
    if len(sys.argv) <= 2:
        start_port = 0
        end_port = 65535
    else:
        portstrs = sys.argv[2].split('-')
        start_port = int(portstrs[0])
        end_port = int(portstrs[1])

    target_ip = gethostbyname(host)
    lock = threading.Lock()
    for port in range(start_port, end_port):
        Scanner(port).start()