#!/usr/bin/env python

import sys
import os
import socket
import signal

sys.path.append(os.path.abspath(os.path.curdir) + "/../../include")
from data_type import type_dict

class music_srv:
        "Music Server class."
        port = 6666
        host = "127.0.0.1"
        socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        message = bytearray(1024)

        def __init__(self):
                self.socket.bind((self.host, self.port))
                signal.signal(signal.SIGINT, self.kill)

        def kill(self, signum, frame):
                self.socket.close()
                sys.exit()

        def run(self):
                while True:
                        nbytes, address = self.socket.recvfrom_into(self.message)
                        if not nbytes:
                                continue
                        data = self.message.decode('utf-8')
                        data = data.split('\0')[0]
                        data_list = data.split('\r\n')
                        if data_list[0] == type_dict['login']:
                               result = self.login(data_list[1], data_list[2])
                               self.response(result)
                                
        def response(self, result):
                send_buffer = result + "\r\n"
                self.socket.sendto()

        def login(self, user, password):
                return "login_success"

srv = music_srv()
srv.run()