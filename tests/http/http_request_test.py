#!/usr/bin/python
from socket import *
import sys


requests = [
    "PUSSY /hello HTTP/1.1\r\n\r\n",
    "\0",
    "\r\n",
    "\r\n\r\n",
    "\r\n\r\nNyaaaa",
    "\rGET",
    "GET /hello HTTP1.1\r",
    "ciao!",
    "GET /hello HTTP/1.1\r\nUser-Agent test\r\n",
    "GET /he\0llo HTTP/1.1",
    "GET /he\0llo HTTP/1.1\r\n\r\n",
    "GET /hello HTTP/1.1\nUser-Agent: test\n\r",
    "GET /h/ e /l /lo HTTP/1.1\r\nUser-Agent: test\r\n\r\n",
    "GET /hello HTTP/1.1\r\nUser-Agent test\r\n\r\n",
    "GET /hello HTTP/1.1\r\nUser-Agent: test\r\n",
    "GET /hello HTTP/1.2\r\nUser-Agent: test\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n",
    "GET /hello HTTP/1.1 \r\nUser-Agent: test\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n",
    "GET /hello HTTP/1.1\r\n\r\n",
    "GET /hello HTTP/1.1\r\nUser-Agent: test\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n",
    "POST /hello HTTP/1.1\r\nUser-Agent: test\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: Keep-Alive\r\n\r\ntest body"
]


def sockSend(data):
    sock = socket(AF_INET, SOCK_STREAM)
    sock.connect( ("127.0.0.1", 8080) )
    print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
    print("Sent:\n" + data)
    sock.send(data)
    print("Got: \n" + sock.recv(1024).split("\r\n")[0])
    sock.close()


try:

    for r in requests:
        sockSend(r)

finally:
    print >>sys.stderr, 'Done!'