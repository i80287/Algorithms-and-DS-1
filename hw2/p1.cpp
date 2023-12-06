#include <cstdint>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

struct Interval {
    int left;
    int right;

    inline constexpr size_t length() const {
        return size_t(int64_t(right) - left + 1);
    }

    inline constexpr Interval overlap(const Interval& other) const {
        auto l = std::max(left, other.left);
        auto r = std::min(right, other.right);
        if (l > r) {
            l = r + 1;
        }

        return { l, r };
    }
};

using std::vector, std::cin, std::cout;

static Interval Solve(const vector<Interval>& ints, uint32_t l, uint32_t r) {
    assert(l <= r && r < ints.size());
    switch (r - l) {
        case 0: {
            return ints.at(l);
        }
        case 1: {
            return ints.at(l).overlap(ints.at(r));
        }
        case 2: {
            auto i1 = ints.at(l).overlap(ints[l + 1]);
            auto i2 = ints[l].overlap(ints.at(l + 2));
            auto i3 = ints[l + 1].overlap(ints[l + 2]);
            auto ans_len = std::max(i1.length(), std::max(i2.length(), i3.length()));
            if (i1.length() == ans_len) {
                return i1;
            }
            else if (i2.length() == ans_len) {
                return i2;
            }
            else {
                return i3;
            }
        }
    }

    auto m = (l + r) / 2;
    Interval left_longest = Solve(ints, l, m);
    Interval right_longest = Solve(ints, m + 1, r);
    assert(left_longest.length() != right_longest.length() || left_longest.left < right_longest.left);

    Interval from_left_part_with_rightest_border = ints[0];
    for (size_t i = l; i <= m; i++) {
        if (ints[i].right > from_left_part_with_rightest_border.right) {
            from_left_part_with_rightest_border = ints[i];
        }
    }

    Interval ans = left_longest.length() >= right_longest.length() ? left_longest : right_longest;
    size_t ans_len = ans.length();
    for (size_t i = m + 1; i <= r; i++) {
        Interval tmp = ints[i].overlap(from_left_part_with_rightest_border);
        size_t tmp_len = tmp.length();
        if (tmp_len > ans_len
        || (tmp_len == ans_len && tmp.left < ans.left)) {
            ans = tmp;
            ans_len = tmp_len;
        }
    }

    return ans;
}

int main() {
    uint32_t n = 0;
    scanf("%u", &n);
    if (n == 0) {
        putchar('0');
        return 0;
    }

    vector<Interval> intervals(n);
    for (auto& i : intervals) {
        scanf("%d %d", &i.left, &i.right);
    }

    if (n == 1) {
        putchar('0');
        return 0;
    }

    std::sort(intervals.begin(), intervals.end(), [](const Interval& i1, const Interval& i2) constexpr -> bool { return i1.left < i2.left || (i1.left == i2.left && i1.right < i2.right); });
    Interval ans = Solve(intervals, 0, n - 1);
    if (ans.length() != 0) {
        printf("%u\n%d %d", uint32_t(ans.length()), ans.left, ans.right);
    }
    else {
        putchar('0');
    }
}
