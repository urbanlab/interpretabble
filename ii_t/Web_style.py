import websockets
import asyncio
import os
import sys

class Server(object):

    def __init__(self, hostname, port):
        self.hostname = hostname
        self.port = port

    async def echo(self, web, path):
        async for messages in websockets:
            await websockets.send(messages)

class Client(object):

    def __init__(self, hostname, port):
        self.hostname = hostname
        self.port = port

    async def hello(self, uri):
        async with websockets.connect(uri) as websocket:
            await websocket.send("Hello world")

if __name__ == '__main__':

    if sys.argv[3] == '-s':
        server = Server(sys.argv[1], sys.argv[2])
        while 1:
            asyncio.get_event_loop().run_until_complete(websockets.serve(server.echo, server.hostname, server.port))
    elif sys.argv[3] == '-c':
        client = Client(sys.argv[1], sys.argv[2])
        while 1:
            asyncio.get_event_loop().run_until_complete(client.hello('ws://' + client.hostname + ':' + client.port))