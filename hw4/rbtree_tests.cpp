#include <cassert>
#include "RBTree.cpp"
#include <set>

int main() {
    RBTree<int> t;
    std::set<int> checker;
    assert(t.empty());
    assert(t.size() == 0);
    assert(t.find(1) == nullptr);
    assert(t.lowerBound(1) == nullptr);

    constexpr const int nums[] = { 1, 2, -3, 4, 0, -4, 35, -45, 20, 23, 22, 21, -15, -28, 56, 57, 44, 69, 72, 101, 118, 114, -114, -118, -101, 13, -13, 12, -12 };
    constexpr const int not_in_nums[] = { -100, -50, -10, 10, 100, 200, 300, 400 };
    for (int num : nums) {
        t.insert(num);
        checker.insert(num);
        assert(t.size() == ssize_t(checker.size()));

        for (int find_num : nums) {
            const int* p = t.find(find_num);
            assert(p != nullptr && *p == find_num);
            if (find_num == num) {
                break;
            }
        }

        for (int find_num : not_in_nums) {
            assert(t.find(find_num) == nullptr);
        }

        for (int lb_num : nums) {
            const int* p = t.lowerBound(lb_num);
            auto correct_ans_iter = checker.lower_bound(lb_num);
            if (correct_ans_iter == checker.end()) {
                assert(p == nullptr);
            } else {
                assert(p != nullptr);
                assert(*p == *correct_ans_iter);
            }
        }
    }
}
