import sys

class Action:
    def __init__(self, x, y, t):
        self.x = x
        self.y = y
        self.t = t


class Task:
    def ReadFromFile(self, filename):
        with open(filename, "r") as fin:
            self.ReadFromStream(fin)

    def ReadFromStream(self, stream=sys.stdin):
        self.t = int(stream.readline().strip())
        self.n, self.m, self.wmax = map(int, stream.readline().strip().split(' '))
        self.p = list(map(int, stream.readline().strip().split(' ')))
    
    def Score(self, w):
        if w <= 0:
            return 0.
        if w >= self.wmax:
            return 10.
        return 10 * (w / self.wmax) ** 2 

    def Evaluate(self, solution):
        '''Returns -1 if sequence of actions is invalid or too big'''
        if len(solution) > 500000:
            return -1
        a = [[-1 for j in range(self.m)] for i in range(self.n)]
        for action in solution:
            if action.x <= 0 or action.x > self.n or action.y <= 0 or action.y > self.m:
                return -1
            if action.t < 0 or action.t > 4:
                return -1
            used = [0 for _ in range(action.t)]
            directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]
            for direction in directions:
                nx, ny = action.x - 1 + direction[0], action.y - 1 + direction[1]
                if nx < 0 or ny < 0 or nx >= self.n or ny >= self.m:
                    continue
                if a[nx][ny] == -1 or a[nx][ny] >= action.t:
                    continue
                used[a[nx][ny]] = 1
            if 0 in used:
                return -1
            a[action.x - 1][action.y - 1] = action.t
        score = 0
        for row in a:
            for value in row:
                if value != -1:
                    score += self.p[value]
        return score

    def PrintSolutonToFile(self, solution, filename):
        '''Prints solution and returns it's points on test'''
        with open(filename, "w") as fout:
            return self.PrintSolutonToStream(solution, fout)
        
    def PrintSolutonToStream(self, solution, stream=sys.stdout):
        '''Prints solution and returns it's points on test'''
        w = self.Evaluate(solution)
        stream.write("{} {}\n".format(w, len(solution)))
        for action in solution:
            stream.write("{} {} {}\n".format(action.x, action.y, action.t))
        return self.Score(w)


if __name__ == "__main__":
    total_points = 0.
    for test in range(1, 11):
        task = Task()
        task.ReadFromFile("tests/input{}.txt".format(test))
        solution = []
        for i in range(task.n):
            for j in range(task.m):
                solution.append(Action(i + 1, j + 1, 0))
        test_points = task.PrintSolutonToFile(solution, "output{}.txt".format(test))
        print("Test {}: {:.4f}".format(test, test_points))
        total_points += test_points
    print("Total: {:.4f}".format(total_points))