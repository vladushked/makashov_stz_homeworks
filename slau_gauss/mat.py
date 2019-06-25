class Mat:
    a = []
    b = []
    a_copy = []
    b_copy = []
    roots = []
    inverse = []

    def __init__(self, elements):
        self.a = [[0] * elements for i in range(elements)]
        self.roots = [[0] * elements for i in range(elements)]
        self.b = [[0] for i in range(elements)]
        self.inverse = [[0] * elements for i in range(elements)]
        for i in range(elements):
            for j in range(elements):
                if i == j:
                    self.inverse[i][j] = 1

    def input(self):
        """input mat A and mat B"""
        print('\nEnter matrix A(' + str(len(self.a)) + 'x' + str(len(self.a[0])) + '):\n')
        for i in range(len(self.a)):
            for j in range(len(self.a[0])):
                self.a[i][j] = int(input('Element A' + str(i) + str(j) + ':'))
        print('\nEnter matrix B(' + str(len(self.a)) + 'x0' + '):\n')
        for i in range(len(self.b)):
            for j in range(len(self.b[0])):
                self.b[i][j] = int(input('Element A' + str(i) + str(j) + ':'))
        """make A and B copy"""
        self.a_copy = self.a.copy()
        self.b_copy = self.b.copy()
        for i in range(len(self.a)):
            self.a_copy[i] = self.a[i].copy()
            self.b_copy[i] = self.b[i].copy()

    def solve(self):
        """choose main element"""
        rows = 0
        cols = 0
        while True:
            if rows >= 3 or cols >= 3:
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
                    self.b[i + 1][0] -= m * self.b[cols][0]
                    for j in range(len(self.a[0])):
                        new_val = self.a[i + 1][j] - m * self.a[cols][j]
                        self.a[i + 1][j] = new_val
        """Xn"""
        self.roots[len(self.a) - 1] = self.b[len(self.b) - 1][0]
        """other roots"""
        for i in range(1, len(self.a[0])):
            self.roots[len(self.a) - 1 - i] = (self.b[len(self.b) - 1 - i][0] - self.a[len(self.a) - 1 - i][
                len(self.a) - 1]) / self.a[len(self.a) - 1 - i][len(self.a) - 1 - i]
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

    def print_mat(self):
        for i in range(len(self.a)):
            print(str(self.a[i]) + '\n')
        for i in range(len(self.b)):
            print(str(self.b[i]) + '\n')
        for i in range(len(self.b)):
            print(str(self.inverse[i]) + '\n')
