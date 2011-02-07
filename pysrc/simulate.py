import time
import zmq
import array

import pyglet
from pyglet.gl import *
from pyglet.window import key

from wavefield import Wavefield

WIDTH  = 100
HEIGHT = 100

SCALE_FACTOR = 4

drawing = True
window = pyglet.window.Window(SCALE_FACTOR * WIDTH, SCALE_FACTOR * HEIGHT)

glPointSize(SCALE_FACTOR)
glScalef(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR)

@window.event
def on_key_press(symbol, modifiers):
    global drawing

    if symbol == key.SPACE:
        drawing = not drawing
    elif symbol == key.ESCAPE:
        window.has_exit = True

@window.event
def on_draw():
    if drawing:
        window.clear()
        wavefield.draw()

def update(dt):
    data.send(str(dt))
    state = array.array('d', data.recv())
    if drawing:
        wavefield.update(state)

pyglet.clock.schedule_interval(update, 1/30.)

wavefield = Wavefield(WIDTH, HEIGHT)

context = zmq.Context(1)

comm = context.socket(zmq.REQ)
comm.bind('tcp://127.0.0.1:5000')

data = context.socket(zmq.REQ)
data.bind('tcp://127.0.0.1:5001')

if __name__ == '__main__':
    pyglet.app.run()
