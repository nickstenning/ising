import array
import pyglet

class Wavefield(object):
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.points = [(x,y) for x in range(self.width) for y in range(self.height)]
        self.image_data = None

    def update(self, data):
        self.data = data

        data_array = array.array(
            'B',
            (int(255 * (x + 1) / 2.0) for x in self.data)
        )

        self.image_data = pyglet.image.ImageData(self.width, self.height, "L", data_array.tostring())

    def draw(self):
        if self.image_data is not None:
            self.image_data.blit(0, 0)