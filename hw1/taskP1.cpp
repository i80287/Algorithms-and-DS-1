#include <atomic>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility> // std::swap
#include <vector>

class DoublyLinkedList {
public:
    using value_type = int; // value_type is part of the general C++ container traits

    struct Node {
        Node* next_ = nullptr;
        Node* prev_ = nullptr;
        std::uint64_t list_id_ = 0;
        value_type value = 0;
    };

    Node* head = nullptr;
    Node* tail = nullptr;

    DoublyLinkedList() noexcept;
    explicit DoublyLinkedList(const std::vector<value_type>& elements);
    DoublyLinkedList(const DoublyLinkedList& other);

    void pushBack(const value_type& element);
    void pushFront(const value_type& element);
    void insert(Node* prev, const value_type& data) noexcept(false);
    void popFront() noexcept(false);
    void popBack() noexcept(false);
    void pop(Node* pos) noexcept(false);
    void erase() noexcept;
    void reverse() noexcept;
    void removeDuplicates();
    void replace(const value_type& el, const value_type& new_element);

    explicit DoublyLinkedList(std::initializer_list<value_type> elements);
    ~DoublyLinkedList();
protected:
    static volatile std::atomic_uint64_t lists_counter;
    // Unique id of the current DoublyLinkedList
    std::uint64_t list_id_ = ++lists_counter;

    template<typename Iterator>
    void initializeFrom(Iterator begin, Iterator end);

    DoublyLinkedList& operator=(const DoublyLinkedList& other);

    DoublyLinkedList(DoublyLinkedList&& other) noexcept;
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;

    inline Node* allocateAlone(const value_type& value) {
        return new Node {
            .next_ = nullptr,
            .prev_ = nullptr,
            .list_id_ = list_id_,
            .value = value
        };
    }

    inline Node* allocateWithPrev(Node* prev, const value_type& value) {
        return new Node {
            .next_ = nullptr,
            .prev_ = prev,
            .list_id_ = list_id_,
            .value = value
        };
    }

    inline Node* allocateWithNext(Node* next, const value_type& value) {
        return new Node {
            .next_ = next,
            .prev_ = nullptr,
            .list_id_ = list_id_,
            .value = value
        };
    }

    inline Node* allocateWithBoth(Node* next, Node* prev, const value_type& value) {
        return new Node {
            .next_ = next,
            .prev_ = prev,
            .list_id_ = list_id_,
            .value = value
        };
    }
};

volatile std::atomic_uint64_t DoublyLinkedList::lists_counter = 0;

DoublyLinkedList::DoublyLinkedList() noexcept {
}

DoublyLinkedList::DoublyLinkedList(const std::vector<value_type>& elements) {
    initializeFrom(elements.begin(), elements.end());
}

DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList& other) {
    Node* other_ptr = other.head;
    if (other_ptr == nullptr) {
        assert(other.tail == nullptr);
        return;
    }

    Node* current = head = allocateAlone(other_ptr->value);
    for (other_ptr = other_ptr->next_; other_ptr != other.tail; other_ptr = other_ptr->next_) {
        assert(other_ptr != nullptr);
        Node* next = current->next_ = allocateWithPrev(current, other_ptr->value);
        current = next;
    }

    tail = current;
}

void DoublyLinkedList::pushBack(const value_type& element) {
    Node* new_node = allocateWithPrev(tail, element);
    if (tail) {
        tail->next_ = new_node;
    }
    else {
        assert(head == nullptr);
        head = new_node;
    }

    tail = new_node;
}

void DoublyLinkedList::pushFront(const value_type& element) {
    Node* new_node = allocateWithNext(head, element);
    if (head) {
        head->prev_ = new_node;
    }
    else {
        assert(tail == nullptr);
        tail = new_node;
    }

    head = new_node;
}

void DoublyLinkedList::insert(DoublyLinkedList::Node* prev, const value_type& data) noexcept(false) {
    if (prev == nullptr || prev->list_id_ != list_id_) {
        throw std::runtime_error("Wrong position for insertion!");
    }

    Node* next = prev->next_;
    Node* new_node = allocateWithBoth(next, prev, data);
    if (next) {
        next->prev_ = new_node;
    }

    prev->next_ = new_node;
}

void DoublyLinkedList::popFront() noexcept(false) {
    if (head == nullptr) {
        throw std::runtime_error("Deletion error!");
    }

    bool one_elements_list = head == tail;
    Node* new_head = head->next_;
    delete head;
    head = new_head;
    if (one_elements_list) {
        assert(new_head == nullptr);
        tail = nullptr;
    }
}

void DoublyLinkedList::popBack() noexcept(false) {
    if (tail == nullptr) {
        throw std::runtime_error("Deletion error!");
    }

    bool one_element_list = head == tail;
    Node* new_tail = tail->prev_;
    delete tail;
    tail = new_tail;
    if (one_element_list) {
        assert(new_tail == nullptr);
        head = nullptr;
    }
}

void DoublyLinkedList::pop(Node* pos) noexcept(false) {
    if (pos == nullptr || pos->list_id_ != list_id_) {
        throw std::runtime_error("Wrong position for insertion!");
    }

    Node* prev = pos->prev_;
    Node* next = pos->next_;
    delete pos;

    if (prev) {
        prev->next_ = next;
    }

    if (next) {
        next->prev_ = prev;
    }
}

void DoublyLinkedList::erase() noexcept {
    for (Node* ptr = head; ptr != tail; ) {
        assert(ptr != nullptr);
        Node* next = ptr->next_;
        delete ptr;
        ptr = next;
    }

    head = nullptr;
    delete tail;
    tail = nullptr;
}

void DoublyLinkedList::reverse() noexcept {
    Node* current_node = head;
    if (current_node == tail) { // size() <= 1
        return;
    }

    assert(current_node != nullptr && tail != nullptr);
    do {
        Node* new_prev = current_node->next_;
        Node* new_next = current_node->prev_;
        current_node->prev_ = new_prev;
        current_node->next_ = new_next;
        current_node = new_prev;
    } while (current_node != nullptr);
    std::swap(head, tail);
}

void DoublyLinkedList::removeDuplicates() {
    Node* current_node = head;
    if (current_node == tail) { // size() <= 1
        return;
    }

    assert(current_node != nullptr && tail != nullptr);

    // can we use std::unordered_set<value_type>?
    static_assert(std::is_integral_v<value_type>);
    struct BinTrie {
        struct BinTrieNode {
            size_t sons_indexes[2] = { static_cast<size_t>(-1), static_cast<size_t>(-1) };
            bool is_terminal_ = false;
        };

        std::vector<BinTrieNode> nodes_;
        constexpr BinTrie() : nodes_{1} {
        }

        // We know that value_type == int, it doesn't depend on List.size() => 
        // => T_tryAdd(elem) = 32 * c = O(1)
        bool tryAdd(value_type elem) {
            size_t node_index = 0;
            auto number = static_cast<std::make_unsigned_t<value_type>>(elem);
            for ( ; number != 0; number >>= 1) {
                size_t son_index = nodes_[node_index].sons_indexes[number & 1];
                if (son_index == static_cast<size_t>(-1)) {
                    break;
                }

                node_index = son_index;
            }

            for ( ; number != 0; number >>= 1) {
                nodes_.emplace_back();
                size_t new_index = nodes_.size() - 1;
                nodes_[node_index].sons_indexes[number & 1] = new_index;
                node_index = new_index;
            }

            bool was_in_tree = nodes_[node_index].is_terminal_;
            nodes_[node_index].is_terminal_ = true;
            return !was_in_tree;
        }

        bool contains(value_type elem) const {
            size_t node_index = 0;
            for (auto number = static_cast<std::make_unsigned_t<value_type>>(elem); number != 0; number >>= 1) {
                size_t son_index = nodes_[node_index].sons_indexes[number & 1];
                if (son_index == static_cast<size_t>(-1)) {
                    return false;
                }

                node_index = son_index;
            }

            return nodes_.at(node_index).is_terminal_;
        }
    } binary_trie;

    do {
        Node* next_node = current_node->next_;
        if (!binary_trie.tryAdd(current_node->value)) {
            Node* prev = current_node->prev_;
            Node* next = current_node->next_;
            if (current_node == head) {
                head = next;
            }

            if (current_node == tail) {
                tail = prev;
            }

            delete current_node;

            if (prev) {
                prev->next_ = next;
            }

            if (next) {
                next->prev_ = prev;
            }
        }

        current_node = next_node;
    } while (current_node != nullptr);
}

void DoublyLinkedList::replace(const value_type& el, const value_type& new_element) {
    for (Node* current_node = head; current_node != nullptr; current_node = current_node->next_) {
        if (current_node->value == el) {
            current_node->value = new_element;
        }
    }
}

DoublyLinkedList::~DoublyLinkedList() {
    erase();
}

DoublyLinkedList::DoublyLinkedList(std::initializer_list<value_type> elements) {
    initializeFrom(elements.begin(), elements.end());
}

template<typename Iterator>
void DoublyLinkedList::initializeFrom(Iterator begin, Iterator end) {
    if (begin == end) {
        return;
    }

    Node* current = head = allocateAlone(*begin);
    for (++begin; begin != end; ++begin) {
        Node* next = current->next_ = allocateWithPrev(current, *begin);
        current = next;
    }

    tail = current;
}

int main() {
    DoublyLinkedList lst{1, 2, 1, 1, 2, 2, 2, 1, 1, 3, 2, 3, 4, 4, 3, 4 };
    lst.removeDuplicates();
    for (auto p = lst.head; p != nullptr; p = p->next_) {
        printf("%d ", p->value);
    }

    return 0;
}
