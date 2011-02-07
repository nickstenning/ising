from itertools import chain, product

from pyglet.gl import *

class Wavefield(object):
    def __init__(self, width, height):
        self.width = width
        self.height = height

        self.vlist = pyglet.graphics.vertex_list(self.width * self.height, 'v2f/static', 'c3f/stream')

        self.points = [(x,y) for x in range(self.width) for y in range(self.height)]
        self.points_offset = [(x + 0.5, y + 0.5) for (x, y) in self.points]
        self.vlist.vertices = list(chain.from_iterable(self.points_offset))

    def update(self, data):
        self.data = data

        self.colors = [self._color(x, y) for (x, y) in self.points]
        self.vlist.colors = list(chain.from_iterable(self.colors))

    def draw(self):
        self.vlist.draw(pyglet.gl.GL_POINTS)

    def _color(self, x, y):
        v = (self.data[self.width * y + x] + 1.0) / 2.0
        return (v, v, v)