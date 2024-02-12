#include "splay-tree.cpp"
#include <set>

[[maybe_unused]] static void CheckSortedInserts() {
    SplayTree t;
    for (int i = -128, h = 0; i <= 127; i++, h++) {
        t.insert(i);
        assert(t.getHeight() == h);
        for (int j = -128; j <= i; j++) {
            const Node* p = t.find(j);
            assert(p != nullptr && p->key == j);
        }
    }

    t.splay(t.find(0));
    assert(t.root_->key == 0);
}

[[maybe_unused]] static void CheckWithstdset() {
    SplayTree t;
    for (int i : {-2, 0, 2}) { assert(t.find(i) == nullptr); }
    assert(t.getHeight() == 0);

    std::set<int> checker;
    const int nums[] = { 1, 2, 3, -1, 2, 3, -2, 5, 1, 6, 3, 8, 8, 7, -1, 1, 23, 12, 6, 32, 5, 12, 2, 2, 2, 1, 3, 6, 4, 8, 6 };
    for (int i : nums) {
        t.insert(i);
        checker.insert(i);
        for (int j : nums) {
            auto iter = checker.find(j);
            Node* p = t.find(j);
            if (iter == checker.end()) {
                assert(p == nullptr);
            } else {
                assert(p != nullptr && p->key == j);
            }
        }
    }
}

inline constexpr int32_t kNullKey = int32_t(-(1u << 31));

[[maybe_unused]] static void CheckNode(const Node* node, int height, int key, int parent_key, int left_key, int right_key) noexcept {
    assert((key == kNullKey) == (node == nullptr));
    if (node == nullptr) {
        return;
    }

    assert(node->key == key);
    assert(node->height_ == height);

    const Node* parent = node->parent;
    assert((parent_key == kNullKey) == (parent == nullptr));
    if (parent != nullptr) {
        assert(parent->key == parent_key);
    }

    const Node* left = node->left;
    assert((left_key == kNullKey) == (left == nullptr));
    if (left != nullptr) {
        assert(left->key == left_key);
    }

    const Node* right = node->right;
    assert((right_key == kNullKey) == (right == nullptr));
    if (right != nullptr) {
        assert(right->key == right_key);
    }
}

[[maybe_unused]] static void CheckPrecise() {
    SplayTree t;
    t.insert(1);
    assert(t.getHeight() == 0);
    CheckNode(t.root_, 0, 1, kNullKey, kNullKey, kNullKey);

    t.insert(2);
    assert(t.getHeight() == 1);
    CheckNode(t.root_, 1, 1, kNullKey, kNullKey, 2);
    CheckNode(t.root_->right, 0, 2, 1, kNullKey, kNullKey);

    t.insert(3);
    assert(t.getHeight() == 2);
    CheckNode(t.root_, 2, 1, kNullKey, kNullKey, 2);
    CheckNode(t.root_->right, 1, 2, 1, kNullKey, 3);
    CheckNode(t.root_->right->right, 0, 3, 2, kNullKey, kNullKey);

    assert(t.splay(t.root_->right) == 1);
    CheckNode(t.root_, 1, 2, kNullKey, 1, 3);
    CheckNode(t.root_->left, 0, 1, 2, kNullKey, kNullKey);
    CheckNode(t.root_->right, 0, 3, 2, kNullKey, kNullKey);
    assert(t.getHeight() == 1);

    t.insert(4);
    CheckNode(t.root_, 2, 2, kNullKey, 1, 3);
    CheckNode(t.root_->left, 0, 1, 2, kNullKey, kNullKey);
    CheckNode(t.root_->right, 1, 3, 2, kNullKey, 4);
    CheckNode(t.root_->right->right, 0, 4, 3, kNullKey, kNullKey);
    assert(t.getHeight() == 2);

    t.insert(5);
    CheckNode(t.root_, 3, 2, kNullKey, 1, 3);
    CheckNode(t.root_->left, 0, 1, 2, kNullKey, kNullKey);
    CheckNode(t.root_->right, 2, 3, 2, kNullKey, 4);
    CheckNode(t.root_->right->right, 1, 4, 3, kNullKey, 5);
    CheckNode(t.root_->right->right->right, 0, 5, 4, kNullKey, kNullKey);
    assert(t.getHeight() == 3);

    t.insert(6);
    CheckNode(t.root_, 4, 2, kNullKey, 1, 3);
    CheckNode(t.root_->left, 0, 1, 2, kNullKey, kNullKey);
    CheckNode(t.root_->right, 3, 3, 2, kNullKey, 4);
    CheckNode(t.root_->right->right, 2, 4, 3, kNullKey, 5);
    CheckNode(t.root_->right->right->right, 1, 5, 4, kNullKey, 6);
    CheckNode(t.root_->right->right->right->right, 0, 6, 5, kNullKey, kNullKey);
    assert(t.getHeight() == 4);

    for (int i : { -2, -1, 0, 7, 8 }) {
        assert(t.find(i) == nullptr);
        CheckNode(t.root_, 4, 2, kNullKey, 1, 3);
        CheckNode(t.root_->left, 0, 1, 2, kNullKey, kNullKey);
        CheckNode(t.root_->right, 3, 3, 2, kNullKey, 4);
        CheckNode(t.root_->right->right, 2, 4, 3, kNullKey, 5);
        CheckNode(t.root_->right->right->right, 1, 5, 4, kNullKey, 6);
        CheckNode(t.root_->right->right->right->right, 0, 6, 5, kNullKey, kNullKey);
        assert(t.getHeight() == 4);
    }

    assert(t.splay(t.root_->right->right->right) == 3);
    assert(t.getHeight() == 3);
    CheckNode(t.root_, 3, 5, kNullKey, 2, 6);
    CheckNode(t.root_->left, 2, 2, 5, 1, 4);
    CheckNode(t.root_->left->left, 0, 1, 2, kNullKey, kNullKey);
    CheckNode(t.root_->left->right, 1, 4, 2, 3, kNullKey);
    CheckNode(t.root_->left->right->left, 0, 3, 4, kNullKey, kNullKey);
    CheckNode(t.root_->right, 0, 6, 5, kNullKey, kNullKey);
}

int main() {
    CheckPrecise();
    CheckWithstdset();
    CheckSortedInserts();
}
