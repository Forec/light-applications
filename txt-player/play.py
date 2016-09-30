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

import os
import time
import argparse
parser = argparse.ArgumentParser()

parser.add_argument('--dir', type=str, default="output")
parser.add_argument('--frame', type=int, default=24)
args = parser.parse_args()

VEDIODIR = args.dir
FRAME = args.frame

os.chdir(VEDIODIR)
files = os.listdir(os.getcwd())
for filename in files:
	file = open(filename, "r")
	os.system("cls")
	lines = file.readlines()
	txt = "".join(lines)
	print(txt)
	time.sleep(1.0 / FRAME)