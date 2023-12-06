#define NDEBUG 1
#define LOCAL_DEBUG 0

#include <cstdint>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <cmath>
#include <cassert>
#include <chrono>

using std::vector, std::cin, std::cout;
using i32 = int32_t;
using i64 = int64_t;
using u32 = uint32_t;
using f128 = long double;

inline constexpr f128 INF = 1.18973149535723176502e+4932L;

#if LOCAL_DEBUG
static u32 l_bin_srch_win = 0;
static u32 l_naive_srch_win = 0;
static u32 r_bin_srch_win = 0;
static u32 r_naive_srch_win = 0;
#endif

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)


struct point_t {
    i32 x;
    i32 y;

    inline constexpr bool operator<(const point_t& other) noexcept {
        return x < other.x || (x == other.x && y < other.y);
    }

    inline constexpr point_t() noexcept : x(0), y(0) {
    }

    inline constexpr point_t(i32 a, i32 b) noexcept : x(a), y(b) {
    }

    inline f128 Distance(const point_t& other) noexcept {
        i64 dx = i64(x) - other.x;
        i64 dy = i64(y) - other.y;
        return std::sqrt(f128(dx * dx + dy * dy));
    }
};

[[maybe_unused]] static inline size_t binsearch_left_point(const vector<point_t>& points, size_t l_x, size_t m_x, f128 left_border) {
    assert(l_x < m_x && left_border <= points[m_x].x);
    size_t l = l_x;
    size_t r = m_x;
#if NDEBUG
#else
    i32 LIMIT = 500;
#endif
    while (l != r
#if NDEBUG
#else
    && --LIMIT >= 0
#endif
    ) {
        auto m = (l + r) / 2;
        auto m_x_x = points[m].x;
        if (m_x_x < left_border) {
            l = m + 1;
        }
        else if (m_x_x != left_border) {
            r = m;
        }
        else {
            assert(l_x <= m);
            if (m == l_x || points[m - 1].x != left_border) {
                return m;
            }

            r = m - 1;
            while (r > l + 1
#if NDEBUG
#else
    && --LIMIT >= 0
#endif
            ) {
                m = (l + r) / 2;
                m_x_x = points[m].x;
                assert(m_x_x <= left_border);
                if (m_x_x == left_border)  {
                    r = m;
                }
                else {
                    l = m + 1;
                }
            }

            if (points[l].x != left_border) {
                assert(l + 1 < m_x);
                l++;
            }
            assert(points[l].x == left_border);
        }
    }

#if NDEBUG
#else
    if (LIMIT < 0 || !(points[l].x >= left_border)) {
        printf("[l_x; m_x] = [%zu, %zu]\n", l_x, m_x);
        printf("[l; r] = [%zu, %zu]\n", l, r);
        for (size_t i = l_x; i <= m_x; i++) {
            printf("{ %d, %d },\n", points[i].x, points[i].y);
        }

        printf("p[l] = %d, p[l+1] = %d, left_border = %.4LF\n", points[l].x, points[l + 1].x, left_border);
        if (LIMIT < 0) {
            printf("LIMIT\n");
        }
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    assert(l_x <= l);
    assert(points[l].x >= left_border);
    assert(l == l_x || points[l - 1].x < left_border);
#endif

    return l;
}

[[maybe_unused]] static inline size_t naive_left_point(const vector<point_t>& points, size_t l_x, size_t m_x, f128 left_border) {
    auto l = m_x;
    while (l >= l_x + 1 && points[l - 1].x >= left_border) {
        l--;
    }

    return l;
}

[[maybe_unused]] static inline size_t binsearch_right_point(const vector<point_t>& points, size_t m_x, size_t r_x, f128 right_border) {
    assert(m_x < r_x && points[m_x].x <= right_border);
    size_t l = m_x;
    size_t r = r_x;
#if NDEBUG
#else
    i32 LIMIT = 500;
#endif
    while (l < r
#if NDEBUG
#else
    && --LIMIT >= 0
#endif
) {
        auto m = (l + r + 1) / 2;
        auto m_x_x = points[m].x;
        if (m_x_x > right_border) {
            assert(m != 0);
            r = m - 1;
        }
        else if (m_x_x != right_border) {
            l = m;
        }
        else {
            if (m == r_x || points[m + 1].x != right_border) {
                return m;
            }

            l = m + 1;
            while (r > l + 1
#if NDEBUG
#else
    && --LIMIT >= 0
#endif
            ) {
                m = (r + l + 1) / 2;
                if (points[m].x == right_border) {
                    l = m;
                }
                else {
                    assert(m != 1);
                    r = m - 1;
                }
            }

            if (l + 1 < r_x && points[l + 1].x == right_border) {
                l++;
            }

            assert(points[l].x == right_border);
        }
    }

#if NDEBUG
#else
    if (LIMIT < 0
        || !(points[l].x <= right_border)
        || !(l == r_x || points[l + 1].x > right_border)) {
        printf("[m_x; r_x] = [%zu, %zu]\n", m_x, r_x);
        printf("[l; r] = [%zu, %zu]\n", l, r);
        for (size_t i = m_x; i <= r_x; i++) {
            printf("\t{ %d, %d },\n", points[i].x, points[i].y);
        }

        printf("l = %zu, r = %zu\n", l, r);
        printf("p[l] = %d, p[l+1] = %d, right_border = %.4LF\n", points[l].x, points[l + 1].x, right_border);
        if (LIMIT < 0) {
            printf("LIMIT\n");
        }
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    assert(points[l].x <= right_border);
    assert(l == r_x || points[l + 1].x > right_border);
#endif

    return l;
}

[[maybe_unused]] static inline size_t naive_right_point(const vector<point_t>& points, size_t m_x, size_t r_x, f128 right_border) {
    size_t r = m_x;
    while (r + 1 <= r_x && points[r + 1].x <= right_border) {
        r++;
    }

    return r;
}

[[maybe_unused]] static f128 MinDistSlow(const vector<point_t>& points, size_t l, size_t r) {
    assert(l <= r && r < points.size());
    f128 dist = INF;
    for (size_t i = l; i < r; i++) {
        auto p = points[i];
        for (size_t j = i + 1; j <= r; j++) {
            dist = std::min(dist, p.Distance(points[j]));
        }
    }

    return dist;
}

[[maybe_unused]] static inline f128 MinDistSlow(const vector<point_t>& points) {
    return MinDistSlow(points, 0, points.size() - 1);
}

static inline void LeftToRightToUp(const vector<point_t>& points, size_t l, size_t m_x, size_t r, f128& min_dist);
static inline void LeftToRightToDown(const vector<point_t>& points, ssize_t l, ssize_t m_x, ssize_t r, f128& min_dist);
static inline void RightToLeftToUp(const vector<point_t>& points, size_t l, size_t m_x, size_t r, f128& min_dist);
static inline void RightToLeftToDown(const vector<point_t>& points, ssize_t l, ssize_t m_x, ssize_t r, f128& min_dist);

static f128 MinDist(const vector<point_t>& points, size_t l_x, size_t r_x) {
    assert(l_x <= r_x && r_x < points.size());
    // printf("MinDist([%zu; %zu]) = %.10LF\n", l_x, r_x, MinDistSlow(points, l_x, r_x));

    if (unlikely(r_x - l_x <= 16)) {
        f128 dist = INF;
        for (size_t i = l_x; i < r_x; i++) {
            auto p_i = points[i];
            for (size_t j = i + 1; j <= r_x; j++) {
                dist = std::min(dist, p_i.Distance(points[j]));
            }
        }

        return dist;
    }

    auto m_x = (l_x + r_x) / 2;
    auto min_dist_left = MinDist(points, l_x, m_x);
    assert(min_dist_left == MinDistSlow(points, l_x, m_x));
    auto min_dist_right = MinDist(points, m_x + 1, r_x);
    assert(min_dist_right == MinDistSlow(points, m_x + 1, r_x));

    f128 min_dist = std::min(min_dist_left, min_dist_right);

    auto mittel_point = points[m_x];
    auto left_border = std::max(mittel_point.x - min_dist, f128(points[l_x].x));
    auto right_border = std::min(mittel_point.x + min_dist, f128(points[r_x].x));

#if LOCAL_DEBUG
    auto start_left_bin = std::chrono::high_resolution_clock::now();
#endif
    size_t l = naive_left_point(points, l_x, m_x, left_border); // binsearch_left_point(points, l_x, m_x, left_border);
#if LOCAL_DEBUG
    auto end_left_bin = std::chrono::high_resolution_clock::now();
    auto start_left_naive = std::chrono::high_resolution_clock::now();
    size_t __dbg_l = naive_left_point(points, l_x, m_x, left_border);
    auto end_left_naive = std::chrono::high_resolution_clock::now();
    auto l_bin = (end_left_bin - start_left_bin).count();
    auto l_naive = (end_left_naive - start_left_naive).count();
    l_bin_srch_win += (l_naive > l_bin);
    l_naive_srch_win += (l_naive < l_bin);
    // if (m_x - l_x + 1 >= 100000) {
    //     printf("%llu, %llu\n", l_bin, l_naive);
    //     printf("left size = %zu\n", m_x - l_x + 1);
    // }
    // if (l_naive < l_bin) {
    //     printf("L Naive win for size %zu\n", m_x - l_x + 1);
    // }
    // else if (l_naive > l_bin) {
    //     printf("L Bin win for size %zu\n", m_x - l_x + 1);
    // }
    if (!(l == __dbg_l)) {
        printf("[l_x; m_x] = [%zu, %zu]\n", l_x, m_x);
        for (size_t i = l_x; i <= m_x; i++) {
            printf("\t{ %d, %d },\n", points[i].x, points[i].y);
        }

        printf("l = %zu, __dbg_l = %zu, left_border = %.4LF\n", l, __dbg_l, left_border);
        printf("p[l] = %d, p[l+1] = %d\n", points[l].x, points[l + 1].x);
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    assert(l == __dbg_l);
#endif
    assert(l_x <= l && l <= m_x);

#if LOCAL_DEBUG
    auto start_right_bin = std::chrono::high_resolution_clock::now();
#endif
    size_t r = naive_right_point(points, m_x, r_x, right_border); //binsearch_right_point(points, m_x, r_x, right_border);
#if LOCAL_DEBUG
    auto end_right_bin = std::chrono::high_resolution_clock::now();
    auto start_right_naive = std::chrono::high_resolution_clock::now();
    size_t __dbg_r = naive_right_point(points, m_x, r_x, right_border);
    auto end_right_naive = std::chrono::high_resolution_clock::now();
    auto r_bin = (end_right_bin - start_right_bin).count();
    auto r_naive = (end_right_naive - start_right_naive).count();
    r_bin_srch_win += r_naive > r_bin;
    r_naive_srch_win += r_naive < r_bin;
    // if (r_x - m_x + 1 >= 100000) {
    //     printf("%llu, %llu\n", r_bin, r_naive);
    //     printf("left size = %zu\n", r_x - m_x + 1);
    // }
    // if (r_naive < r_bin) {
    //     printf("R Naive win for size %zu\n", r_x - m_x + 1);
    // }
    // else if (r_naive > r_bin) {
    //     printf("R Bin win for size %zu\n", r_x - m_x + 1);
    // }
    if (!(r == __dbg_r)) {
        printf("[m_x; r_x] = [%zu, %zu]\n", m_x, r_x);
        for (size_t i = m_x; i <= r_x; i++) {
            printf("\t{ %d, %d },\n", points[i].x, points[i].y);
        }

        printf("r = %zu, __dbg_r = %zu, right_border = %.4LF\n", r, __dbg_r, right_border);
        printf("p[r] = %d, p[r+1] = %d\n", points[r].x, points[r + 1].x);
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    assert(r == __dbg_r);
#endif
    assert(m_x <= r && r <= r_x);
    if (likely(m_x != r)) {
        assert(l < r);
        LeftToRightToUp(points, l, m_x, r, min_dist);
        LeftToRightToDown(points, ssize_t(l), ssize_t(m_x), ssize_t(r), min_dist);
        RightToLeftToUp(points, l, m_x, r, min_dist);
        RightToLeftToDown(points, ssize_t(l), ssize_t(m_x), ssize_t(r), min_dist);
    }

    return min_dist;
}

static inline void LeftToRightToUp(const vector<point_t>& points, size_t l, size_t m_x, size_t r, f128& min_dist) {
    size_t window_j_l = m_x + 1;
    size_t window_j_r = window_j_l + 5;

    for (size_t i = l; i <= m_x; i++) {
        auto p_i = points[i];
        while (window_j_l <= r && p_i.Distance(points[window_j_l]) > min_dist) {
            window_j_l++;
            window_j_r++;
        }

        size_t border_j = std::min(r, window_j_r);
        for (size_t j = window_j_l; j <= border_j; j++) {
            min_dist = std::min(min_dist, p_i.Distance(points.at(j)));
        }
    }
}

static inline void LeftToRightToDown(const vector<point_t>& points, ssize_t l, ssize_t m_x, ssize_t r, f128& min_dist) {
    ssize_t window_j_r = r;
    ssize_t window_j_l = window_j_r - 5;

    for (ssize_t i = m_x; i >= l; i--) {
        auto p_i = points[i];
        while (window_j_r >= m_x + 1 && p_i.Distance(points[window_j_r]) > min_dist) {
            window_j_r--;
            window_j_l--;
        }

        ssize_t border_j = std::max(m_x + 1, window_j_l);
        for (ssize_t j = window_j_r; j >= border_j; j--) {
            min_dist = std::min(min_dist, p_i.Distance(points.at(j)));
        }
    }
}

static inline void RightToLeftToUp(const vector<point_t>& points, size_t l, size_t m_x, size_t r, f128& min_dist) {
    size_t window_i_l = l;
    size_t window_i_r = window_i_l + 5;

    for (size_t j = m_x + 1; j <= r; j++) {
        auto p_j = points[j];
        while (window_i_l <= m_x && p_j.Distance(points[window_i_l]) > min_dist) {
            window_i_l++;
            window_i_r++;
        }

        size_t border_i = std::min(m_x, window_i_r);
        for (size_t i = window_i_l; i <= border_i; i++) {
            min_dist = std::min(min_dist, p_j.Distance(points.at(i)));
        }
    }
}

static inline void RightToLeftToDown(const vector<point_t>& points, ssize_t l, ssize_t m_x, ssize_t r, f128& min_dist) {
    ssize_t window_i_r = m_x;
    ssize_t window_i_l = window_i_r - 5;

    for (ssize_t j = r; j >= m_x + 1; j--) {
        auto p_j = points[j];
        while (window_i_r >= l && p_j.Distance(points[window_i_r]) > min_dist) {
            window_i_r--;
            window_i_l--;
        }

        ssize_t border_i = std::max(l, window_i_l);
        for (ssize_t i = window_i_r; i >= border_i; i--) {
            min_dist = std::min(min_dist, p_j.Distance(points.at(i)));
        }
    }
}

[[maybe_unused]] static inline f128 MinDist(const vector<point_t>& points) {
    return MinDist(points, 0, points.size() - 1);
}

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <random>

namespace Oder {

struct Point {
    int x;
    int y;
    constexpr Point() : x{}, y{} {}
    constexpr Point(int a, int b) : x(a), y(b) {}

    constexpr int GetX() const {
        return x;
    }

    constexpr int GetY() const {
        return y;
    }

    inline int FindDist(Point b) const {
        return int(sqrt((x - b.GetX()) * (x - b.GetX()) + (y - b.GetY()) * (y - b.GetY())));
    }
};

// int FindMiddleX(std::vector<Point> vect) {
    /*int sum_x = 0;
    for (Point p: vect) {
        sum_x += p.GetX();
    }
    if (vect.size() == 0) {   // а что делать с пустым вектором
        return 0;
    }
    return sum_x / vect.size();*/
// }

std::mt19937 gen(std::random_device{}());

//  Функция выбора pivot
int pivot_fn(const std::vector<Point>& l) {
    std::uniform_int_distribution<> dis(0, int(l.size() - 1));
    int pivot = dis(gen);
    return l[pivot].GetX();
}


// Функция для поиска k-го элемента в списке
int quickselect(const std::vector<Point>& l, int k, int (*pivot_fn)(const std::vector<Point>&)) {
    // Если длина списка равна 1, то возвращаем единственный элемент
    if (l.size() == 1) {
        return l[0].GetX();
    }

    int pivot = pivot_fn(l);

    // Разделяем список на три части: элементы меньше pivot, равные pivot и больше pivot
    std::vector<Point> lows, highs, pivots;
    for (int i = 0; i < int(l.size()); i++) {
        if (l[i].GetX() < pivot) {
            lows.push_back(l[i]);
        }
        else if (l[i].GetX() > pivot) {
            highs.push_back(l[i]);
        }
        else {
            pivots.push_back(l[i]);
        }
    }

    // Если k меньше длины списка lows, то рекурсивно вызываем функцию для списка lows
    if (k < int(lows.size())) {
        return quickselect(lows, k, pivot_fn);
    }
    // Если k находится между длинами списков lows и pivots, то возвращаем первый элемент из списка pivots
    else if (k < int(lows.size() + pivots.size())) {
        return pivots[0].GetX();
    }
    // Если k больше длины списка lows и pivots, то рекурсивно вызываем функцию для списка highs
    else {
        return quickselect(highs, int(k - lows.size() - pivots.size()), pivot_fn);
    }
}

// Функция для поиска медианы
double quickselect_median(const std::vector<Point>& l, int k, int (*pivot_fn)(const std::vector<Point>&)) {
    // Если длина списка нечетная, то медианой будет элемент с индексом k
    if (l.size() % 2 == 1) {
        return quickselect(l, k, pivot_fn);
    }
        // Если длина списка четная, то медианой будет среднее значение двух элементов с индексами k-1 и k
    else {
        return 0.5 * (quickselect(l, k-1, pivot_fn) + quickselect(l, k, pivot_fn));
    }
}

bool comparePoints(Point a, Point b) {
    return a.GetY() < b.GetY();
}

int GetClosestPair(const std::vector<Point>& vect) {
    if (vect.size() == 1) {  // (vect.size() == 0
        return 2147483640;
    }
    if (vect.size() == 2) {
        return vect[0].FindDist(vect[1]);
    }
    std::vector<Point> left_points;
    std::vector<Point> right_points;
    double mid_x = quickselect_median(vect, int(vect.size()/2), pivot_fn);
    for (Point p: vect) {
        if (p.GetX() < mid_x) {
            left_points.push_back(p);
        } else {
            right_points.push_back(p);
        }
    }

    int min_left = GetClosestPair(left_points);
    int min_right = GetClosestPair(right_points);

    std::vector<Point> left_points_apgr;
    std::vector<Point> right_points_apgr;

    int min_delta = std::min(min_left, min_right);
    int left_bound = int(mid_x - min_delta);
    int right_bound = int(mid_x + min_delta);
    for (Point p: left_points) {
        if (p.GetX() >= left_bound) {
            left_points_apgr.push_back(p);
        }
    }
    for (Point p: right_points) {
        if (p.GetX() < right_bound) {    // попробовать заменить на <=
            right_points_apgr.push_back(p);
        }
    }

    std::sort(left_points_apgr.begin(), left_points_apgr.end(), comparePoints);
    std::sort(right_points_apgr.begin(), right_points_apgr.end(), comparePoints);

    for (Point p: left_points_apgr) {
        int i = 0; // индекс начала просмотра ближайших элементов
        /*while (right_points_apgr.size() > 0 && p.FindDist(right_points_apgr[0]) > min_delta) {
            ++i;
        }*/
        int size_of_right = int(right_points_apgr.size());
        for (; i < std::min(size_of_right, 6); ++i) { // может на 3 заменить
            if (p.FindDist(right_points_apgr[i]) < min_delta) {  // можно вынести в отд переменную расстояние
                min_delta = p.FindDist(right_points_apgr[i]);
            }
        }
    }

    return min_delta;
}

}

#include <random>

[[maybe_unused]] static inline void AnsTests() {
    std::mt19937 rnd; // (std::random_device{}());
#if NDEBUG
    constexpr int32_t K = 4096;
#else
    constexpr int32_t K = 2048;
#endif
    constexpr int32_t PointsCoordMod = 64;
    vector<point_t> points(size_t(1e6));
    vector<Oder::Point> oder_points(size_t(1e6));
    for (size_t n = 2; n <= K; n++) {
        points.resize(n);
        oder_points.resize(n);
        size_t i = 0;
        printf("n = %zu\n", n);
        for (point_t& p : points) {
            p.x = i32(rnd()) % PointsCoordMod;
            p.y = i32(rnd()) % PointsCoordMod;
            oder_points[i].x = p.x;
            oder_points[i].y = p.y;
            i++;
            cout << p.x << ' ' << p.y << '\n';
        }

        std::sort(points.begin(), points.end());
        // auto dist1 = MinDist(points);
        auto dist3 = Oder::GetClosestPair(oder_points);
        auto dist2 = MinDistSlow(points);
        if (dist3 != int(dist2)) {
            printf("n = %zu\nans fast = %.10LF\nans slow = %.10LF\n", n, f128(dist1), dist2);
            for (point_t p : points) {
                printf("\t{ %d, %d },\n", p.x, p.y);
            }
            return;
        }
    }
}

[[maybe_unused]] static inline void TimeTests() {
    std::mt19937 rnd(std::random_device{}());
    constexpr int32_t n = 1'000'000;
    vector<point_t> points(n);
    constexpr int32_t PointsCoordModBigN = int32_t(1e9);
    for (int32_t i = 0; i < 32; i++) {
        for (point_t& p : points) {
            p.x = i32(rnd()) % PointsCoordModBigN;
            p.y = i32(rnd()) % PointsCoordModBigN;
        }

        std::sort(points.begin(), points.end());
        auto start = std::chrono::high_resolution_clock::now();
        auto ans = MinDist(points);
        auto end = std::chrono::high_resolution_clock::now();
        printf("%lld ms (%d)\n", (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()), int(ans));
    }

    int32_t y = -n / 2;
    for (size_t i = 0; i < n / 2; i++) {
        points[i].x = -1;
        points[i].y = y++;
    }
    for (size_t i = n / 2; i < n; i++) {
        points[i].x = 1;
        points[i].y = y++;
    }

    std::sort(points.begin(), points.end());
    auto start = std::chrono::high_resolution_clock::now();
    auto ans = MinDist(points);
    auto end = std::chrono::high_resolution_clock::now();
    printf("%lld ms (%d)\n", (long long)(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()), int(ans));
}

[[maybe_unused]] static inline void SpecialTest() {
    constexpr int32_t n = 24;
    vector<point_t> points(n);

    int32_t y = -n;
    for (size_t i = 0; i < n / 2; i++) {
        points[i].x = -1;
        points[i].y = y;
        y += 4;
    }
    y = -n;
    for (size_t i = n / 2; i < n; i++) {
        points[i].x = 1;
        points[i].y = y;
        y += 4;
    }

    std::sort(points.begin(), points.end());
    auto ans = MinDist(points);
    assert(ans == 2);
    printf("%d\n", int(ans));
}

[[maybe_unused]] static inline void Tests() {
    AnsTests();
    SpecialTest();
    // TimeTests();
}

[[maybe_unused]] static inline void Console() {
    vector<point_t> points;
    points.reserve(size_t(1e5)); i32 a = 0, b = 0; while (scanf("%d %d", &a, &b) == 2) { points.emplace_back(a, b); }
    assert(!points.empty());
    std::sort(points.begin(), points.end());
    printf("%d\n", int(MinDist(points)));
#if LOCAL_DEBUG
    printf("[>>>] %d\n", int(MinDistSlow(points)));
#endif
}

int main() {
    if constexpr (false) {
        Tests();
#if LOCAL_DEBUG
        printf("l_bin_srch_win = %u\n"
               "l_naive_srch_win = %u\n"
               "r_bin_srch_win = %u\n"
               "r_naive_srch_win = %u\n", l_bin_srch_win, l_naive_srch_win, r_bin_srch_win, r_naive_srch_win);
#endif
    }
    else {
        Console();
    }
}
