#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <array>
#include <cassert>

struct Action {
  Action() = default;

  Action(int x, int y, int t) : x(x), y(y), t(t) {
  }

  int x, y, t;
};

struct Task {
  void Read(const std::string& filename) {
    std::ifstream fin(filename);
    assert(fin.is_open());
    Read(fin);
  }

  void Read(std::istream& in = std::cin) {
    in >> test;
    in >> n >> m >> wmax;
    for (int i = 0; i < 5; i++) {
      in >> p[i];
    }
  }

  // Returns -1 if sequence of actions is invalid or too big
  int Evaluate(const std::vector<Action>& solution) {
    if (solution.size() > 500000) {
      return -1;
    }
    std::vector<std::vector<int>> a(n, std::vector<int>(m, -1));
    for (const auto& action : solution) {
      if (action.x <= 0 || action.x > n || action.y <= 0 || action.y > m) {
        return -1;
      }
      if (action.t < 0 || action.t > 4) {
        return -1;
      }
      std::vector<int> used(action.t);
      constexpr std::pair<int, int> dirs[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
      for (const auto& dir : dirs) {
        int nx = action.x - 1 + dir.first;
        int ny = action.y - 1 + dir.second;
        if (nx < 0 || nx >= n) {
          continue;
        }
        if (ny < 0 || ny >= m) {
          continue;
        }
        if (a[nx][ny] == -1 || a[nx][ny] >= action.t) {
          continue;
        }
        used[a[nx][ny]] = 1;
      }
      if (std::find(used.begin(), used.end(), 0) != used.end()) {
        return -1;
      }
      a[action.x - 1][action.y - 1] = action.t;
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
      return 10;
    }
    return (10. * w / wmax) * (1. * w / wmax);
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
    out << w << " " << solution.size() << "\n";
    for (const auto& action : solution) {
      out << action.x << " " << action.y << " " << action.t << "\n";
    }
    return Score(w);
  }

  int test{};
  int n{};
  int m{};
  int wmax{};
  std::array<int, 5> p{};
};

int main(int argc, char* argv[]) {
  double total_points = 0;
  for (int test = 1; test <= 10; test++) {
    Task task;
    task.Read("tests/input" + std::to_string(test) + ".txt");
    std::vector<Action> solution;
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