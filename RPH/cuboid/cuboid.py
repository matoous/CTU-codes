class Cuboid:
    def __init__(self, h1, h2, h3):
        self.h = [h1, h2, h3]

    def compute_volume(self):
        volume = 1
        for x in self.h:
            volume *= x
        return volume

    def make_scaled_copy(self, scale):
        h2 = [x*scale for x in self.h]
        return Cuboid(h2[0], h2[1], h2[2])
