#include "BTree.cpp"

int main() {
    BTree t(2);
    assert(t.size() == 0);
    assert(t.sum() == 0);
    t.insert(1);
    /**
     * +---+
     * | 1 |
     * +---+
     */
    assert(t.sum() == 1);
    t.insert(1);
    /**
     * +---+
     * | 1 |
     * +---+
     */
    assert(t.size() == 1);
    assert(t.sum() == 1);
    t.insert(3);
    /**
     * +---+---+
     * | 1 | 3 |
     * +---+---+
     */
    assert(t.size() == 1);
    assert(t.sum() == 4);
    t.insert(-1);
    /**
     * +----+---+---+
     * | -1 | 1 | 3 |
     * +----+---+---+
     */
    assert(t.size() == 1);
    assert(t.sum() == 3);
    t.insert(-1);
    /**
     * +----+---+---+
     * | -1 | 1 | 3 |
     * +----+---+---+
     */
    assert(t.size() == 1);
    assert(t.sum() == 3);
    t.insert(2);
    /**
     *      +---+
     *      | 1 |
     *      +---+
     *     /     \
     * +----+ +---+---+
     * | -1 |_| 2 | 3 |
     * +----+ +---+---+
     */
    assert(t.size() == 3);
    assert(t.sum() == 4);
    assert(t.nodes_[t.root_index_].keys_size_ == 1);
    assert(t.nodes_[t.root_index_].keys_[0] == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[0] == -1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[1]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[0] == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[1] == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].next_in_row_index_ ==
           0);
    t.insert(5);
    /**
     *      +---+
     *      | 1 |
     *      +---+
     *     /     \
     * +----+ +---+---+---+
     * | -1 |_| 2 | 3 | 5 |
     * +----+ +---+---+---+
     */
    assert(t.size() == 3);
    assert(t.sum() == 9);
    assert(t.nodes_[t.root_index_].keys_size_ == 1);
    assert(t.nodes_[t.root_index_].keys_[0] == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[0] == -1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[1]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_size_ == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[0] == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[1] == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[2] == 5);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].next_in_row_index_ ==
           0);
    t.insert(6);
    /**
     *      +---+---+
     *      | 1 | 3 |
     *      +---+---+
     *     /    |    \
     * +----+ +---+ +---+---+
     * | -1 |_| 2 |_| 5 | 6 |
     * +----+ +---+ +---+---+
     */
    assert(t.size() == 4);
    assert(t.sum() == 12);
    assert(t.nodes_[t.root_index_].keys_size_ == 2);
    assert(t.nodes_[t.root_index_].keys_[0] == 1);
    assert(t.nodes_[t.root_index_].keys_[1] == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[0] == -1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[1]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[0] == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[2]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[0] == 5);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[1] == 6);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].next_in_row_index_ ==
           0);
    t.insert(7);
    /**
     *      +---+---+
     *      | 1 | 3 |
     *      +---+---+
     *     /    |    \
     * +----+ +---+ +---+---+---+
     * | -1 |_| 2 |_| 5 | 6 | 7 |
     * +----+ +---+ +---+---+---+
     */
    assert(t.size() == 4);
    assert(t.sum() == 19);
    t.insert(-2);
    /**
     *           +---+---+
     *           | 1 | 3 |
     *           +---+---+
     *          /    |    \
     * +----+----+ +---+ +---+---+---+
     * | -2 | -1 |_| 2 |_| 5 | 6 | 7 |
     * +----+----+ +---+ +---+---+---+
     */
    assert(t.size() == 4);
    assert(t.sum() == 17);
    t.insert(4);
    /**
     *              +---+---+---+
     *              | 1 | 3 | 6 |
     *           ___+---+---+---+____
     *          /      /     \       \
     * +----+----+ +---+ +---+---+ +---+
     * | -2 | -1 |_| 2 |_| 4 | 5 |_| 7 |
     * +----+----+ +---+ +---+---+ +---+
     */
    assert(t.size() == 5);
    assert(t.sum() == 15);
    assert(t.nodes_[t.root_index_].keys_size_ == 3);
    assert(t.nodes_[t.root_index_].keys_[0] == 1);
    assert(t.nodes_[t.root_index_].keys_[1] == 3);
    assert(t.nodes_[t.root_index_].keys_[2] == 6);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[0] == -2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[1] == -1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[1]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[0] == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[2]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[0] == 4);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[1] == 5);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[3]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_[0] == 7);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].next_in_row_index_ ==
           0);
    t.insert(0);
    /**
     *                +---+---+---+
     *                | 1 | 3 | 6 |
     *           _____+---+---+---+____
     *          /         |    \       \
     * +----+----+---+ +---+ +---+---+ +---+
     * | -2 | -1 | 0 |_| 2 |_| 4 | 5 |_| 7 |
     * +----+----+---+ +---+ +---+---+ +---+
     */
    assert(t.size() == 5);
    assert(t.sum() == 15);
    assert(t.nodes_[t.root_index_].keys_size_ == 3);
    assert(t.nodes_[t.root_index_].keys_[0] == 1);
    assert(t.nodes_[t.root_index_].keys_[1] == 3);
    assert(t.nodes_[t.root_index_].keys_[2] == 6);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_size_ == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[0] == -2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[1] == -1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[2] == 0);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[1]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[0] == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[2]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[0] == 4);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[1] == 5);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[3]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_[0] == 7);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].next_in_row_index_ ==
           0);
    t.insert(11);
    /**
     *                +---+---+---+
     *                | 1 | 3 | 6 |
     *           _____+---+---+---+____
     *          /         |    \       \
     * +----+----+---+ +---+ +---+---+ +---+----+
     * | -2 | -1 | 0 |_| 2 |_| 4 | 5 |_| 7 | 11 |
     * +----+----+---+ +---+ +---+---+ +---+----+
     */
    assert(t.size() == 5);
    assert(t.sum() == 26);
    assert(t.nodes_[t.root_index_].keys_size_ == 3);
    assert(t.nodes_[t.root_index_].keys_[0] == 1);
    assert(t.nodes_[t.root_index_].keys_[1] == 3);
    assert(t.nodes_[t.root_index_].keys_[2] == 6);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_size_ == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[0] == -2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[1] == -1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[2] == 0);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[1]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[0] == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[2]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[0] == 4);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[1] == 5);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[3]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_[0] == 7);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_[1] == 11);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].next_in_row_index_ ==
           0);
    t.insert(100);
    /**
     *                +---+---+---+
     *                | 1 | 3 | 6 |
     *           _____+---+---+---+____
     *          /         |    \       \
     * +----+----+---+ +---+ +---+---+ +---+----+-----+
     * | -2 | -1 | 0 |_| 2 |_| 4 | 5 |_| 7 | 11 | 100 |
     * +----+----+---+ +---+ +---+---+ +---+----+-----+
     */
    assert(t.size() == 5);
    assert(t.sum() == 126);
    assert(t.nodes_[t.root_index_].keys_size_ == 3);
    assert(t.nodes_[t.root_index_].keys_[0] == 1);
    assert(t.nodes_[t.root_index_].keys_[1] == 3);
    assert(t.nodes_[t.root_index_].keys_[2] == 6);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_size_ == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[0] == -2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[1] == -1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].keys_[2] == 0);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[1]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_size_ == 1);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].keys_[0] == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[2]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_size_ == 2);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[0] == 4);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].keys_[1] == 5);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[2]].next_in_row_index_ ==
           t.nodes_[t.root_index_].child_indexes_[3]);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_size_ == 3);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_[0] == 7);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_[1] == 11);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].keys_[2] == 100);
    assert(t.nodes_[t.nodes_[t.root_index_].child_indexes_[3]].next_in_row_index_ ==
           0);
    t.insert(15);
    /**
     *                    +---+
     *                    | 3 |
     *                    +---+
     *                    /   \
     *                +---+   +---+----+
     *                | 1 |___| 6 | 11 |
     *           _____+---+   +---+----+_____
     *          /        |      |     \      \
     * +----+----+---+ +---+ +---+---+ +---+ +----+-----+
     * | -2 | -1 | 0 |_| 2 |_| 4 | 5 |_| 7 |_| 15 | 100 |
     * +----+----+---+ +---+ +---+---+ +---+ +----+-----+
     */
    assert(t.size() == 8);
    assert(t.sum() == 130);
    assert(t.nodes_[t.root_index_].keys_size_ == 1);
    assert(t.nodes_[t.root_index_].keys_[0] == 3);
    const auto& node_2_1 = t.nodes_[t.nodes_[t.root_index_].child_indexes_[0]];
    const auto& node_2_2 = t.nodes_[t.nodes_[t.root_index_].child_indexes_[1]];
    assert(node_2_1.keys_size_ == 1);
    assert(node_2_1.keys_[0] == 1);
    assert(node_2_1.next_in_row_index_ == t.nodes_[t.root_index_].child_indexes_[1]);
    assert(node_2_2.keys_size_ == 2);
    assert(node_2_2.keys_[0] == 6);
    assert(node_2_2.keys_[1] == 11);
    assert(node_2_2.next_in_row_index_ == 0);

    const auto& node_3_1 = t.nodes_[node_2_1.child_indexes_[0]];
    const auto& node_3_2 = t.nodes_[node_2_1.child_indexes_[1]];
    const auto& node_3_3 = t.nodes_[node_2_2.child_indexes_[0]];
    const auto& node_3_4 = t.nodes_[node_2_2.child_indexes_[1]];
    const auto& node_3_5 = t.nodes_[node_2_2.child_indexes_[2]];
    assert(node_3_1.keys_size_ == 3);
    assert(node_3_1.keys_[0] == -2);
    assert(node_3_1.keys_[1] == -1);
    assert(node_3_1.keys_[2] == 0);
    assert(node_3_1.child_indexes_[0] == 0);
    assert(node_3_1.child_indexes_[1] == 0);
    assert(node_3_1.child_indexes_[2] == 0);
    assert(node_3_1.child_indexes_[3] == 0);
    assert(node_3_1.next_in_row_index_ == node_2_1.child_indexes_[1]);
    assert(node_3_2.keys_size_ == 1);
    assert(node_3_2.keys_[0] == 2);
    assert(node_3_2.child_indexes_[0] == 0);
    assert(node_3_2.child_indexes_[1] == 0);
    assert(node_3_2.next_in_row_index_ == node_2_2.child_indexes_[0]);
    assert(node_3_3.keys_size_ == 2);
    assert(node_3_3.keys_[0] == 4);
    assert(node_3_3.keys_[1] == 5);
    assert(node_3_3.child_indexes_[0] == 0);
    assert(node_3_3.child_indexes_[1] == 0);
    assert(node_3_3.child_indexes_[2] == 0);
    assert(node_3_3.next_in_row_index_ == node_2_2.child_indexes_[1]);
    assert(node_3_4.keys_size_ == 1);
    assert(node_3_4.keys_[0] == 7);
    assert(node_3_4.child_indexes_[0] == 0);
    assert(node_3_4.child_indexes_[1] == 0);
    assert(node_3_4.next_in_row_index_ == node_2_2.child_indexes_[2]);
    assert(node_3_5.keys_size_ == 2);
    assert(node_3_5.keys_[0] == 15);
    assert(node_3_5.keys_[1] == 100);
    assert(node_3_5.child_indexes_[0] == 0);
    assert(node_3_5.child_indexes_[1] == 0);
    assert(node_3_5.child_indexes_[2] == 0);
    assert(node_3_5.next_in_row_index_ == 0);
}
