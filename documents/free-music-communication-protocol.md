### Free Music 通信协议

#### 协议概述
服务器与客户端之间没有建立稳定的 TCP 连接，而是直接通过 UDP 发送数据包实现相互通信的。客户端直接向服务器发送相关请求（请求登录验证、新用户注册申请、请求获取指定用户的歌单列表、获取指定歌单内歌曲列表，搜索请求等。）然后服务器返回请求，结束后，服务器会发送一个确认信息（登录验证通过、注册成功、列表发送完毕等）。通过这样简单的方式实现双方通信。

#### 协议报文字段
报文内容全部为 utf-8 字符，规定前 4 个字符一定为数字。标志该报文是什么类型的信息。如：“0001\r\nrain.by.zhou@xxx.com\r\npassword” 。该段信息起始类型为 ‘0001’ 表明这是一个登录请求信息。每个字段采用 ‘\r\n’ 分割，因此服务器后台能够成功解析到请求登录的 email 以及相应的登录密码（初步未考虑对密码加密）。再如：服务器后台收到 "0008\r\nrain_of_mem@ddd.com" 根据前四个字符，知道客户正在请求用户 email 为 rain_of_mem@ddd.com 的歌单信息。因此服务器后台会立即查询并发送相应信息。当然这个信息的开头也是 4 位数字的字符串。后面才是对应的字段内容（具体什么内容在在开过程中可慢慢确定，客户端需要什么服务器后台相应给出就是，只需要用 \r\n 分割即可）。

#### 通信过程代码举例
##### 服务器端代码：
```

#!/usr/bin/env python

import sys
import os
import socket
import signal

type_dict = {   'login':                        '0001',
                'login_success':                '0002',
                'login_failed':                 '0003', 
                'register':                     '0004',
                'register_success':             '0005',
                'register_failed':              '0006',
                'fetch_end':                    '0007',
                'fetch_list':                   '0008',
                'fetch_song':                   '0009',
                'new_list':                     '0010',
                'new_list_success':             '0011',
                'new_list_failed':              '0012'

                }

class music_srv:
        "Music Server class."
        port = 6666
        host = "127.0.0.1"
        client_address = ['0.0.0.0', 6666]
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
                        self.client_address = address
                        data = self.message.decode('utf-8')
                        data = data.split('\0')[0]
                        data_list = data.split('\r\n')

                        if data_list[0] == type_dict['login']:
                                result = self.login(data_list[1], data_list[2])
                                self.response(result)
                        elif data_list[0] == type_dict['register']:
                                result = self.register(data_list[1], data_list[2], data_list[3])
                                self.response(result)
                        elif data_list[0] == type_dict['fetch_list']:
                                result = self.send_user_list(data_list[1])
                                self.response(result)
                        elif data_list[0] == type_dict['new_list']:
                                result = self.user_list_new(data_list[1], data_list[2])
                                self.response(result)
                        else:
                                pass
                                
        def response(self, result):
                send_buffer = result.encode('utf-8')
                self.socket.sendto(send_buffer, self.client_address)

        def login(self, email, password):
                return "login_success"
        
        def register(self, email, password, nick):
                return "register_success"

        def send_user_list(self, email):
                return "fetch_end"

        def user_list_new(self, email, list_name): # 响应用户添加新歌单
                return "new_list_success" 

srv = music_srv()
srv.run()
```
##### 客户端代码：
```
!/usr/bin/env python

import socket

server_host = "127.0.0.1"
host = socket.gethostname()
port = 6666
sk = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

_type = "0010"   # 用户添加新歌单请求
user = "rain.by.zhou@gmail.com"
password = "233333"
nick = "rain"
list_name = "like"
message = _type + '\r\n' + user + '\r\n' + list_name
sk.sendto(message.encode('utf-8'), (server_host, port))
byte_buf = bytearray(1024)
sk.recvfrom_into(byte_buf)
data = byte_buf.decode('utf-8')
data = data.split('\0')[0]
data_list = data.split('\r\n')
print(data_list)
sk.close()
```
#### 总结
相关协议还在开发设计中，逐渐会变得清晰明了的。所有协议中提到的消息类型定义在这个文件中：
https://raw.githubusercontent.com/RainMark/Free-Music/python-dev/src/include/data_type.py 
若客户端不是 python 代码。只要保证在发送数据前 utf-8 编码，接受数据后 utf-8 解码即可。
