class MyMatrix:
    def __init__(self, matice = None):
        self.matrix = matice

    def load(self, path):
        self.matrix = []
        with open(path, 'r', encoding="utf-8") as f:
            for line in f:
                self.matrix.append([])
                temp = line.split(',')
                for i in range(1,len(temp)-1):
                    self.matrix[len(self.matrix) - 1].append(int(temp[i]))

    def save(self, path):
        with open(path, 'w', encoding="utf-8") as f:
            for i in self.matrix:
                f.write(str(i))
                f.write("\n")

    def get_matrix(self):
        return self.matrix


if __name__ == "__main__":
    a = MyMatrix([[1, 2, 3], [2, 3, 4]])
    b = MyMatrix()
    a.save('matrix.txt')
    b.load('matrix.txt')
    a.get_matrix() == b.get_matrix()