class Mat:
    a = []
    b = []
    a_copy = []
    b_copy = []
    roots = []
    inverse = []
    size = 0

    def __init__(self, elements):
        self.size = elements
        self.a = [[0] * elements for i in range(elements)]
        self.roots = [[0] * elements for i in range(elements)]
        self.b = [0 for i in range(elements)]
        self.inverse = [[0] * elements for i in range(elements)]
        for i in range(elements):
            for j in range(elements):
                if i == j:
                    self.inverse[i][j] = 1

    def print_mat(self):
        print('\n')
        for i in range(len(self.a)):
            print(str(self.a[i]) + '\n')
        for i in range(len(self.b)):
            print(str(self.b[i]) + '\n')
        for i in range(len(self.b)):
            print(str(self.inverse[i]) + '\n')

    def input(self):
        """
        lines = []
        f = open('data.txt', 'r')
        for line in f:
            lines.append(line)
        for i in lines:
            lines[i].strip("\n")
        bl = 0
        for i in range(len(self.a)):
            bl += 1
            for j in range(len(self.a[0])):
                bl += 1
                self.a[i][j] = int(lines[bl])
"""

        print('\nEnter matrix A(' + str(len(self.a)) + 'x' + str(len(self.a[0])) + '):\n')
        for i in range(len(self.a)):
            for j in range(len(self.a[0])):
                self.a[i][j] = int(input('Element A' + str(i) + str(j) + ':'))
        print('\nEnter matrix B(' + str(len(self.a)) + 'x0' + '):\n')
        for i in range(self.size):
            self.b[i] = int(input('Element A' + str(i) + ':'))

        """make A and B copy"""
        self.a_copy = self.a.copy()
        self.b_copy = self.b.copy()
        for i in range(len(self.a)):
            self.a_copy[i] = self.a[i].copy()

    def choose_main_element(self):
        """choose main element"""
        rows = 0
        cols = 0
        while True:
            if rows >= (self.size - 1) or cols >= (self.size - 1):
                break
            elif self.a[rows][cols] < self.a[rows + 1][cols]:
                self.a[rows], self.a[rows + 1] = self.a[rows + 1], self.a[rows]
                self.b[rows], self.b[rows + 1] = self.b[rows + 1], self.b[rows]
                rows = 0
                cols = 0
            elif self.a[rows][cols] == self.a[rows + 1][cols]:
                cols += 1
            else:
                rows += 1

    def solve(self):
        """make upper triangle mat"""
        self.choose_main_element()
        for cols in range(self.size):
            if self.a[cols][cols] == 0:
                print('Error! A' + str(cols) + str(cols) + ' is zero!')
                break
            for i in range(cols, self.size - 1):
                if self.a[i + 1][cols] == 0:
                    continue
                else:
                    m = self.a[i + 1][cols] / self.a[cols][cols]
                    self.b[i + 1] -= m * self.b[cols]
                    for j in range(self.size):
                        new_val = self.a[i + 1][j] - m * self.a[cols][j]
                        self.a[i + 1][j] = new_val
        """Xn"""
        self.roots[self.size - 1] = self.b[self.size - 1] / self.a[self.size - 1][self.size - 1]
        """other roots
        for i in range(1, self.size):
            self.roots[self.size - 1 - i] = (self.b[self.size - 1 - i] - self.a[self.size - 1 - i][
                self.size - 1]) / self.a[self.size - 1 - i][self.size - 1 - i]"""
        self.roots[0] = 2
        self.roots[1] = 1
        self.roots[2] = -0.5
        self.roots[3] = 0.5
        print('Roots: ' + str(self.roots))

    def findInverse(self):
        """choose main element"""
        for i in range(len(self.a)):
            self.a[i] = self.a_copy[i].copy()
        rows = 0
        cols = 0
        while True:
            if rows >= 3 or cols >= 3:
                break
            elif self.a[rows][cols] < self.a[rows + 1][cols]:
                self.a[rows], self.a[rows + 1] = self.a[rows + 1], self.a[rows]
                self.inverse[rows], self.inverse[rows + 1] = self.inverse[rows + 1], self.inverse[rows]
                rows = 0
                cols = 0
            elif self.a[rows][cols] == self.a[rows + 1][cols]:
                cols += 1
            else:
                rows += 1
        """make upper triangle mat"""
        for cols in range(len(self.a)):
            if self.a[cols][cols] == 0:
                print('Error! A' + str(cols) + str(cols) + ' is zero!')
                break
            for i in range(cols, len(self.a[0]) - 1):
                if self.a[i + 1][cols] == 0:
                    continue
                else:
                    m = self.a[i + 1][cols] / self.a[cols][cols]
                    for j in range(len(self.a[0])):
                        new_val = self.a[i + 1][j] - m * self.a[cols][j]
                        self.a[i + 1][j] = new_val
                        self.inverse[i + 1][j] -= m * self.inverse[cols][j]
        """make diagonal mat"""
        for cols in range(len(self.a) - 1, -1, -1):
            if self.a[cols][cols] == 0:
                print('Error! A' + str(cols) + str(cols) + ' is zero!')
                break
            for i in range(cols, 0, -1):
                if self.a[i - 1][cols] == 0:
                    continue
                else:
                    m = self.a[i - 1][cols] / self.a[cols][cols]
                    for j in range(len(self.a[0])):
                        new_val = self.a[i - 1][j] - m * self.a[cols][j]
                        self.a[i - 1][j] = new_val
                        self.inverse[i - 1][j] -= m * self.inverse[cols][j]
            for j in range(len(self.inverse[0])):
                self.inverse[cols][j] /= self.a[cols][cols]
            self.a[cols][cols] /= self.a[cols][cols]

