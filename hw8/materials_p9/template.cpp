#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

struct Action {
    constexpr Action() noexcept = default;
    constexpr Action(int x, int y, int t) noexcept : x(x), y(y), t(t) {}

    int x = 0;
    int y = 0;
    int t = 0;
};

struct Task {
    void Read(const std::string& filename) {
        std::ifstream fin(filename);
        assert(fin.is_open());
        Read(fin);
    }

    void Read(std::istream& in = std::cin) {
        in >> test;
        if (!in) {
            throw std::runtime_error("test number");
        }
        in >> n >> m >> wmax;
        if (!in) {
            throw std::runtime_error("n m wmax");
        }
        for (auto& p_i : p) {
            in >> p_i;
        }
    }

    // Returns -1 if sequence of actions is invalid or too big
    int Evaluate(const std::vector<Action>& solution) {
        if (solution.size() > 500000) {
            return -1;
        }
        std::vector<std::vector<int>> a(n, std::vector<int>(m, -1));
        for (const auto [x, y, t] : solution) {
            if (x <= 0 || x > n || y <= 0 || y > m) {
                return -1;
            }
            if (t < 0 || t > 4) {
                return -1;
            }
            std::vector<int> used(t);
            constexpr std::pair<int, int> dirs[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            for (const auto [dir_x, dir_y] : dirs) {
                int nx = x - 1 + dir_x;
                int ny = y - 1 + dir_y;
                if (nx < 0 || nx >= n) {
                    continue;
                }
                if (ny < 0 || ny >= m) {
                    continue;
                }
                if (a[nx][ny] == -1 || a[nx][ny] >= t) {
                    continue;
                }
                used[a[nx][ny]] = 1;
            }
            if (std::find(used.begin(), used.end(), 0) != used.end()) {
                return -1;
            }
            a[x - 1][y - 1] = t;
        }
        int score = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j] != -1) {
                    score += p[a[i][j]];
                }
            }
        }
        return score;
    }

    double Score(int w) {
        if (w <= 0) {
            return 0;
        }
        if (w >= wmax) {
            return 2;
        }
        const auto d = double(w) / wmax;
        assert(0 < d && d < 1);
        return 2 * d * d;
    }

    // Prints solution and returns it's points on test
    double PrintSolution(const std::vector<Action>& solution, const std::string& filename) {
        std::ofstream fout(filename);
        assert(fout.is_open());
        return PrintSolution(solution, fout);
    }

    // Prints solution and returns it's points on test
    double PrintSolution(const std::vector<Action>& solution, std::ostream& out = std::cout) {
        int w = Evaluate(solution);
        out << w << ' ' << solution.size() << '\n';
        for (const auto& action : solution) {
            out << action.x << ' ' << action.y << ' ' << action.t << '\n';
        }
        return Score(w);
    }

    int test{};
    int n{};
    int m{};
    int wmax{};
    std::array<int, 5> p{};
};

int main() {
    double total_points = 0;
    for (unsigned test = 1; test <= 10; test++) {
        Task task;
        task.Read("tests/input" + std::to_string(test) + ".txt");
        std::vector<Action> solution;
        solution.reserve(size_t(task.n * task.m));
        for (int i = 0; i < task.n; i++) {
            for (int j = 0; j < task.m; j++) {
                solution.emplace_back(i + 1, j + 1, 0);
            }
        }
        double test_points = task.PrintSolution(solution, "output" + std::to_string(test) + ".txt");
        printf("Test %d: %.4f points\n", test, test_points);
        total_points += test_points;
    }
    printf("Total: %.4f points\n", total_points);
    return 0;
}
