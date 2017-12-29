#!/bin/env python3
from http import server
from http.server import SimpleHTTPRequestHandler

#server.test(protocol="HTTP/1.1")
server.test(HandlerClass = SimpleHTTPRequestHandler,
            protocol="HTTP/1.1",
            port=8000)