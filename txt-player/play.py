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