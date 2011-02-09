import time
import zmq
import array

import pyglet
from pyglet.gl import *
from pyglet.window import key

from bitplane import Bitplane

def update(dt):
    data.send("1")
    state = array.array('h', data.recv())
    bitplane.update(state)

if __name__ == '__main__':
    context = zmq.Context(1)

    comm = context.socket(zmq.REQ)
    comm.bind('tcp://127.0.0.1:5000')

    data = context.socket(zmq.REQ)
    data.bind('tcp://127.0.0.1:5001')

    comm.send("dims")
    width, height = map(int, comm.recv().split())

    window = pyglet.window.Window(width, height)
    bitplane = Bitplane(width, height)

    @window.event
    def on_key_press(symbol, modifiers):
        if symbol == key.SPACE:
            comm.send("rst")
            assert comm.recv() == "OK"
        elif symbol == key.ESCAPE:
            window.has_exit = True

    @window.event
    def on_draw():
        window.clear()
        bitplane.draw()

    pyglet.clock.schedule_interval(update, 1/10.0)
    pyglet.app.run()


