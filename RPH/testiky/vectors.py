import math

class MyVector(object):
    def __init__(self, elements):
        self.elements = elements

    def get_vector(self):
        return self.elements

    def __mul__(self, other):
        return sum([x*y for x,y in zip(self.elements, other.elements)])

    def norm(self):
        return math.sqrt(sum([x**2 for x in self.elements]))

    def __add__(self, other):
        return MyVector([x + y for x, y in zip(self.elements, other.elements)])