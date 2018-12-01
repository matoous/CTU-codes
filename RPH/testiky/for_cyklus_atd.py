def find_max(in_list):
    curr_max = 0
    curr_index = -1
    for i, item in enumerate(in_list):
        if item > curr_max:
            curr_max = item
            curr_index = i
    if curr_index == -1:
        print("Empty field exceptation")
    return curr_index, curr_max


class Point():
    def __init__(self,x,y):
        self.x = x
        self.y = y

    def __mul__(self, other):
        return Point(self.x * other.x, self.y * other.y)

    def __str__(self):
        return '(' + str(self.x) + ',' + str(self.y) + ')'


if __name__ == "__main__":
    a = Point(1,2)
    b = Point(2,3)

    c = b * a
    print(b, "*", a)
    print(c)

