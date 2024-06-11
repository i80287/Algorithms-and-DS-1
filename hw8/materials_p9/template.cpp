#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using std::uint32_t, std::size_t;

struct Action {
    constexpr Action() noexcept = default;
    constexpr Action(int x, int y, int t) noexcept : x(x), y(y), t(t) {}

    int x = 0;
    int y = 0;
    int t = 0;
};

struct Task {
    static Task Read(const std::string& filename) {
        std::ifstream fin(filename);
        assert(fin.is_open());
        return Read(fin);
    }

    static Task Read(std::istream& in = std::cin) {
        if (!in) {
            throw std::runtime_error("bad istream");
        }
        Task t;
        in >> t.test;
        if (!in) {
            throw std::runtime_error("test number");
        }
        in >> t.n >> t.m >> t.wmax;
        if (!in) {
            throw std::runtime_error("n m wmax");
        }
        for (auto& p_i : t.p) {
            in >> p_i;
        }
        return t;
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

void sample() {
    double total_points = 0;
    for (unsigned test = 1; test <= 10; test++) {
        Task task = Task::Read("tests/input" + std::to_string(test) + ".txt");
        std::vector<Action> solution;
        solution.reserve(std::size_t(task.n * task.m));
        for (int i = 0; i < task.n; i++) {
            for (int j = 0; j < task.m; j++) {
                solution.emplace_back(i + 1, j + 1, 0);
            }
        }
        double test_points = task.PrintSolution(solution, "output" + std::to_string(test) + ".txt");
        printf("Test %u: %.4f points\n", test, test_points);
        total_points += test_points;
    }
    printf("Total: %.4f points\n", total_points);
}

void read_tests() {
    for (auto test : {1u, 3u, 5u, 6u}) {
        const auto test_str = std::to_string(test);

        std::ifstream fin("solve/output" + test_str + ".txt");
        int w{};
        uint32_t k{};
        fin >> w >> k;
        if (!fin) {
            std::cerr << "bad test " + test_str + "\n";
            return;
        }

        std::vector<Action> solution(k);
        for (size_t i = 0; i < k; i++) {
            if (!fin) {
                std::cerr << "bad test " + test_str + " | k = " + std::to_string(k) + "\n";
                return;
            }
            Action& act = solution[i];
            fin >> act.x >> act.y >> act.t;
        }

        Task t     = Task::Read("tests/input" + test_str + ".txt");
        int real_w = t.Evaluate(solution);
        std::cout << "Test: " + test_str << " | real_w = " << real_w << " | w = " << w
                  << " | wmax = " << t.wmax << " | score = " << t.Score(real_w) << '\n';
    }
}

int main() {
    // sample();
    read_tests();
    return 0;
}
