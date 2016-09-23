import cv2
import sys
import struct
import pickle
import time
import threading
import argparse
import numpy as np
from socket import *

parser = argparse.ArgumentParser()

parser.add_argument('--host', type=str, default='127.0.0.1')
parser.add_argument('--port', type=int, default=10087)
parser.add_argument('--noself', type=bool, default=False)
parser.add_argument('-v', '--version', type=int, default=4)

args = parser.parse_args()

IP = args.host
PORT = args.port
VERSION = args.version
SHOWME = not args.noself

class Server(threading.Thread):
    def __init__(self, port) :
        print("server starts...")
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.ADDR = ('', port)
        if VERSION == 4:
            self.sock = socket(AF_INET ,SOCK_STREAM)
        else:
            self.sock = socket(AF_INET6 ,SOCK_STREAM)
        self.sock.bind(self.ADDR)
        self.sock.listen(1)
        self.thStop = False
    def __del__(self):
        self.sock.close()
        try:
            cv2.destroyAllWindows()
        except:
            pass
    def run(self):
        conn, addr = self.sock.accept()
        print("client success connected...")
        data = "".encode("utf-8")
        payload_size = struct.calcsize("L")
        cv2.namedWindow('Remote', cv2.WINDOW_NORMAL)
        while True:
            while len(data) < payload_size:
                data += conn.recv(81920)
            packed_size = data[:payload_size]
            data = data[payload_size:]
            msg_size = struct.unpack("L", packed_size)[0]
            while len(data) < msg_size:
                data += conn.recv(81920)
            frame_data = data[:msg_size]
            data = data[msg_size:]
            frame = pickle.loads(frame_data)
            cv2.imshow('Remote', frame)
            if cv2.waitKey(1) & 0xFF == 27:
                break

class Client(threading.Thread):
    def __init__(self ,ip, port):
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.ADDR = (ip, port)
        if VERSION == 4:
            self.sock = socket(AF_INET, SOCK_STREAM)
        else:
            self.sock = socket(AF_INET6, SOCK_STREAM)
        self.cap = cv2.VideoCapture(0)
        self.thStop = False
        print("client starts...")
    def __del__(self) :
        self.sock.close()
        self.cap.release()
        if SHOWME:
            try:
                cv2.destroyAllWindows()
            except:
                pass
    def run(self):
        while True:
            try:
                self.sock.connect(self.ADDR)
                break
            except:
                time.sleep(3)
                continue
        if SHOWME:
            cv2.namedWindow('You', cv2.WINDOW_NORMAL)
        print("client connected...")
        while self.cap.isOpened():
            ret, frame = self.cap.read()
            if SHOWME:
                cv2.imshow('You', frame)
                if cv2.waitKey(1) & 0xFF == 27:
                    break
            data = pickle.dumps(frame)
            try:
                self.sock.sendall(struct.pack("L", len(data)) + data)
            except:
                break

if __name__ == '__main__':
    client = Client(IP, PORT)
    server = Server(PORT)
    client.start()
    server.start()
    while True:
        time.sleep(1)
        if not server.isAlive() or not client.isAlive():
            sys.exit(0)