#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <utility> // std::swap
#include <unordered_set>
#include <vector>

struct Node {
    Node* next;
    Node* previous;
    int data;
    
    Node() : next{nullptr}, previous{nullptr}, data{0} {
    }

    Node(int valiue) : next{nullptr}, previous{nullptr}, data{valiue} {
    }
};

class DoublyLinkedList {
public:
    Node* head = nullptr;
    Node* tail = nullptr;

    DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList& other);
    DoublyLinkedList(std::vector<int>& array);
    ~DoublyLinkedList();

    void pushBack(int data);
    void pushFront(int data);
    void insert(Node* prev, int data);
    void popFront();
    void popBack();
    void pop(Node* pos);
    void erase();
    void reverse();
    void removeDuplicates();
    void replace(int oldElem, int newElem);
private:
    std::unordered_set<Node*> nodes_;
};

DoublyLinkedList::DoublyLinkedList() {
}

DoublyLinkedList::DoublyLinkedList(std::vector<int>& elements) {
    auto begin = elements.cbegin();
    auto end = elements.cend();
    if (begin == end) {
        return;
    }

    Node* current = head = new Node(*begin);
    nodes_.insert(current);
    for (++begin; begin != end; ++begin) {
        Node* next = new Node(*begin);
        nodes_.insert(next);
        next->previous = current;
        current->next = next;
        current = next;
    }

    tail = current;
}

DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList& other) {
    Node* other_ptr = other.head;
    if (other_ptr == nullptr) {
        assert(other.tail == nullptr);
        return;
    }

    Node* current = head = new Node(other_ptr->data);
    nodes_.insert(current);
    for (other_ptr = other_ptr->next; other_ptr != other.tail; other_ptr = other_ptr->next) {
        assert(other_ptr != nullptr);
        Node* next = new Node(other_ptr->data);
        next->previous = current;
        current->next = next;
        current = next;
    }

    tail = current;
}

void DoublyLinkedList::pushBack(int element) {
    Node* new_node = new Node(element);
    nodes_.insert(new_node);
    new_node->previous = tail;
    if (tail) {
        tail->next = new_node;
    }
    else {
        assert(head == nullptr);
        head = new_node;
    }

    tail = new_node;
}

void DoublyLinkedList::pushFront(int element) {
    Node* new_node = new Node(element);
    nodes_.insert(new_node);
    new_node->next = head;
    if (head) {
        head->previous = new_node;
    }
    else {
        assert(tail == nullptr);
        tail = new_node;
    }

    head = new_node;
}

void DoublyLinkedList::insert(Node* prev, int data) {
    if (prev == nullptr || nodes_.find(prev) == nodes_.end()) {
        throw std::runtime_error("Wrong position for insertion!");
    }

    Node* next = prev->next;
    Node* new_node = new Node(data);
    nodes_.insert(new_node);
    new_node->next = next;
    new_node->previous = prev;
    if (next) {
        next->previous = new_node;
    }

    prev->next = new_node;
}

void DoublyLinkedList::popFront() {
    if (head == nullptr) {
        assert(tail == nullptr);
        throw std::runtime_error("Deletion error!");
    }

    bool one_elements_list = head == tail;
    Node* new_head = head->next;
    nodes_.erase(head);
    delete head;
    head = new_head;
    if (one_elements_list) {
        assert(new_head == nullptr);
        tail = nullptr;
    }
}

void DoublyLinkedList::popBack() {
    if (tail == nullptr) {
        throw std::runtime_error("Deletion error!");
    }

    bool one_element_list = head == tail;
    Node* new_tail = tail->previous;
    nodes_.erase(tail);
    delete tail;
    tail = new_tail;
    if (one_element_list) {
        assert(new_tail == nullptr);
        head = nullptr;
    }
}

void DoublyLinkedList::pop(Node* pos) {
    if (pos == nullptr || nodes_.find(pos) == nodes_.end()) {
        throw std::runtime_error("Wrong position for deletion!");
    }

    Node* next = pos->next;
    Node* prev = pos->previous;

    nodes_.erase(pos);
    delete pos;

    if (pos == head) {
        if (next == nullptr) {
            assert(head == tail);
            tail = nullptr;
        }

        head = next;
    }

    if (pos == tail) {
        if (prev == nullptr) {
            assert(head == tail);
            head = nullptr;
        }

        tail = prev;
    }

    if (next) {
        next->previous = prev;
    }

    if (prev) {
        prev->next = next;
    }
}

void DoublyLinkedList::erase() {
    for (Node* ptr = head; ptr != nullptr; ) {
        Node* next = ptr->next;
        delete ptr;
        ptr = next;
    }

    head = nullptr;
    tail = nullptr;
    nodes_.clear();
}

void DoublyLinkedList::reverse() {
    Node* current_node = head;
    if (current_node == tail) { // size() <= 1
        return;
    }

    assert(current_node != nullptr && tail != nullptr);
    do {
        Node* new_prev = current_node->next;
        Node* new_next = current_node->previous;
        current_node->previous = new_prev;
        current_node->next = new_next;
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
    std::unordered_set<int> seen_ints;
    do {
        Node* next_node = current_node->next;
        if (!seen_ints.insert(current_node->data).second) {
            Node* prev = current_node->previous;
            Node* next = current_node->next;
            if (current_node == head) {
                head = next;
            }

            if (current_node == tail) {
                tail = prev;
            }

            nodes_.erase(current_node);
            delete current_node;

            if (prev) {
                prev->next = next;
            }

            if (next) {
                next->previous = prev;
            }
        }

        current_node = next_node;
    } while (current_node != nullptr);
}

void DoublyLinkedList::replace(int oldElem, int newElem) {
    for (Node* current_node = head; current_node != nullptr; current_node = current_node->next) {
        if (current_node->data == oldElem) {
            current_node->data = newElem;
        }
    }
}

DoublyLinkedList::~DoublyLinkedList() {
    try {
        erase();
    } catch (...) {
    }
}

__attribute__((__noinline__, __noreturn__)) void __throw(const char* func_name, const char* file_name, int line_number) noexcept(false) {
    char buffer[256];
    sprintf(buffer, "%s, file %s, line %d\n", func_name, file_name, line_number);
    throw std::runtime_error(buffer);
}

#define Assert(condition) (void)((!!(condition)) || (__throw(__PRETTY_FUNCTION__, __FILE__, __LINE__), 0))

void CheckList(const DoublyLinkedList& lst, const std::vector<int>& vec) {
    Node* current = lst.head;
    if (vec.empty()) {
        Assert(current == nullptr && lst.tail == nullptr);
        return;
    }

    for (int value : vec) {
        Assert(current != nullptr && value == current->data);
        current = current->next;
    }

    Assert(current == nullptr);
}

void RemoveDuplicates(std::vector<int>& vec) {
    std::unordered_set<int> elements;
    elements.reserve(vec.size());
    std::vector<int> new_vec;
    new_vec.reserve(vec.size());
    for (int value : vec) {
        if (elements.insert(value).second) {
            new_vec.push_back(value);
        }
    }

    vec = std::move(new_vec);
}

void Reverse(std::vector<int>& vec) {
    auto it1 = vec.begin();
    auto it2 = vec.end() - 1;
    while (it1 < it2) {
        std::swap(*it1, *it2);
        ++it1;
        --it2;
    }
}

int main() {
    std::vector<int> vec{1, 2, 1, 1, 2, 2, 2, 1, 1, 3, 2, 3, 4, 4, 3, 4 };
    DoublyLinkedList lst{vec};
    CheckList(lst, vec);

    lst.removeDuplicates();
    RemoveDuplicates(vec);
    CheckList(lst, vec);

    lst.reverse();
    Reverse(vec);
    CheckList(lst, vec);

    for (int i = -2; i <= 5; i++) {
        lst.pushBack(i);
        vec.push_back(i);
        CheckList(lst, vec);
        lst.popFront();
        vec.erase(vec.begin());
        CheckList(lst, vec);
    }

    lst.pop(lst.head);
    vec.erase(vec.begin());
    CheckList(lst, vec);

    lst.pop(lst.tail);
    vec.erase(vec.end() - 1);
    CheckList(lst, vec);

    lst.pop(lst.head);
    vec.erase(vec.begin());
    CheckList(lst, vec);

    lst.pop(lst.tail);
    vec.erase(vec.end() - 1);
    CheckList(lst, vec);

    Assert(lst.head == nullptr && lst.tail == nullptr);

    lst.pushBack(1);
    vec.push_back(1);
    CheckList(lst, vec);

    lst.pushFront(2);
    vec.insert(vec.begin(), 2);
    CheckList(lst, vec);

    for (auto p = lst.head; p != nullptr; p = p->next) { printf("%d ", p->data); } putchar('\n');
    for (int elem : vec) { printf("%d ", elem); } putchar('\n');

    return 0;
}
