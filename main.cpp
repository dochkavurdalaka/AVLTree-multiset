#include <algorithm>
#include <stack>
#include <vector>
#include <iostream>

template <class Type>
class AVLTree {
    struct Node {
        Type key = Type{};
        Node* left = nullptr;
        Node* right = nullptr;
        int height = 0;
        size_t count = 1;
        size_t weight = 1;
    };

    Node* root_ = nullptr;

    bool Contains(Node* node, const Type& key) const {
        if (node == nullptr) {
            return false;
        }
        if (key == node->key) {
            return true;
        }
        if (key < node->key) {
            return Contains(node->left, key);
        }
        return Contains(node->right, key);
    }

    int Height(Node* node) const {
        if (node == nullptr) {
            return -1;
        }
        return node->height;
    }

    size_t Weight(Node* node) const {
        if (node == nullptr) {
            return 0;
        }
        return node->weight;
    }

    int Diff(Node* node) {
        return Height(node->left) - Height(node->right);
    }

    void LeftRotate(Node*& node) {
        Node* root_subtree = node;
        node = node->right;
        root_subtree->right = node->left;
        node->left = root_subtree;
        node->left->height = std::max(Height(node->left->left), Height(node->left->right)) + 1;
        node->height = std::max(Height(node->left), Height(node->right)) + 1;

        node->left->weight =
            Weight(node->left->left) + Weight(node->left->right) + node->left->count;
        node->weight = Weight(node->left) + Weight(node->right) + node->count;
    }

    void RightRotate(Node*& node) {
        Node* root_subtree = node;
        node = node->left;
        root_subtree->left = node->right;
        node->right = root_subtree;
        node->right->height = std::max(Height(node->right->left), Height(node->right->right)) + 1;
        node->height = std::max(Height(node->left), Height(node->right)) + 1;

        node->right->weight =
            Weight(node->right->left) + Weight(node->right->right) + node->right->count;
        node->weight = Weight(node->left) + Weight(node->right) + node->count;
    }

    void Insert(Node*& node, const Type& key) {
        if (node == nullptr) {
            node = new Node();
            node->key = key;
            return;
        }
        if (key == node->key) {
            node->count += 1;
            node->weight += 1;
            return;
        }
        if (key < node->key) {
            Insert(node->left, key);
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            node->weight = Weight(node->left) + Weight(node->right) + node->count;
            int diff_b = Diff(node);
            if (diff_b == 2) {
                int diff_a = Diff(node->left);
                if (diff_a == 0 or diff_a == 1) {
                    RightRotate(node);
                } else {
                    LeftRotate(node->left);
                    RightRotate(node);
                }
            }
            return;
        }
        Insert(node->right, key);
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
        node->weight = Weight(node->left) + Weight(node->right) + node->count;
        int diff_a = Diff(node);
        if (diff_a == -2) {
            int diff_b = Diff(node->right);
            if (diff_b == 0 or diff_b == -1) {
                LeftRotate(node);
            } else {
                RightRotate(node->right);
                LeftRotate(node);
            }
        }
    }

    Node* WorkWithInorderSucessor(Node*& node) {
        if (node->left != nullptr) {
            Node* node_to_return = WorkWithInorderSucessor(node->left);
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            node->weight = Weight(node->left) + Weight(node->right) + node->count;
            int diff_a = Diff(node);
            if (diff_a == -2) {
                int diff_b = Diff(node->right);
                if (diff_b == 0 or diff_b == -1) {
                    LeftRotate(node);
                } else {
                    RightRotate(node->right);
                    LeftRotate(node);
                }
            }
            return node_to_return;
        }
        Node* node_to_return = node;
        node = node->right;
        return node_to_return;
    }

    void Remove(Node*& node, const Type& key) {
        if (node == nullptr) {
            return;
        }
        if (key == node->key) {
            if (node->count != 1) {
                node->count -= 1;
                node->weight -= 1;
                return;
            }

            if (node->left == nullptr and node->right == nullptr) {
                delete node;
                node = nullptr;
                return;
            }
            if (node->left == nullptr) {
                Node* node_right = node->right;
                delete node;
                node = node_right;
                return;
            }
            if (node->right == nullptr) {
                Node* node_left = node->left;
                delete node;
                node = node_left;
                return;
            }

            Node* node_returned = WorkWithInorderSucessor(node->right);
            node_returned->left = node->left;
            node_returned->right = node->right;
            delete node;
            node = node_returned;
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            node->weight = Weight(node->left) + Weight(node->right) + node->count;
            int diff_b = Diff(node);
            if (diff_b == 2) {
                int diff_a = Diff(node->left);
                if (diff_a == 0 or diff_a == 1) {
                    RightRotate(node);
                } else {
                    LeftRotate(node->left);
                    RightRotate(node);
                }
            }
            return;
        }
        if (key < node->key) {
            Remove(node->left, key);
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            node->weight = Weight(node->left) + Weight(node->right) + node->count;
            int diff_a = Diff(node);
            if (diff_a == -2) {
                int diff_b = Diff(node->right);
                if (diff_b == 0 or diff_b == -1) {
                    LeftRotate(node);
                } else {
                    RightRotate(node->right);
                    LeftRotate(node);
                }
            }
            return;
        }
        Remove(node->right, key);
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
        node->weight = Weight(node->left) + Weight(node->right) + node->count;
        int diff_b = Diff(node);
        if (diff_b == 2) {
            int diff_a = Diff(node->left);
            if (diff_a == 0 or diff_a == 1) {
                RightRotate(node);
            } else {
                LeftRotate(node->left);
                RightRotate(node);
            }
        }
    }

    void Print(Node* node) {
        if (node == nullptr) {
            return;
        }

        Print(node->left);
        for (size_t i_cnt = 0; i_cnt < node->count; ++i_cnt) {
            std::cout << node->key << " ";
        }
        Print(node->right);
    }

    void Destroy(Node* node) {
        if (node == nullptr) {
            return;
        }
        Destroy(node->left);
        Destroy(node->right);
        delete node;
    }

    Type KOrderStatistic(Node* node, size_t order) {
        size_t weight_left = Weight(node->left);
        if (weight_left < order and order <= weight_left + node->count) {
            return node->key;
        }
        if (order <= weight_left) {
            return KOrderStatistic(node->left, order);
        }
        return KOrderStatistic(node->right, order - (weight_left + node->count));
    }

public:
    bool Contains(const Type& key) const {
        return Contains(root_, key);
    }

    void Insert(const Type& key) {
        Insert(root_, key);
    }

    void Print() {
        Print(root_);
    }
    void Remove(const Type& key) {
        Remove(root_, key);
    }

    void Clear() {
        Destroy(root_);
        root_ = nullptr;
    }

    bool IsEmpty() const {
        return root_ == nullptr;
    }

    size_t Size() const {
        return Weight(root_);
    }

    Type KOrderStatistic(size_t order) {
        return KOrderStatistic(root_, order);
    }

    ~AVLTree() {
        Destroy(root_);
    }
};

int main() {
    //std::cout << multiset.Size() << " ";
    /*multiset.Insert(5);
    multiset.Insert(6);
    multiset.Insert(4);
    multiset.Insert(4);
    multiset.Insert(6);
    multiset.Insert(6);
    multiset.Insert(5);
    multiset.Insert(4);
    multiset.Insert(1);
    multiset.Insert(2);
    multiset.Insert(2);
    multiset.Insert(1);
    multiset.Insert(9);
    multiset.Insert(8);
    multiset.Print();
    std::cout<< "\n";
    std::cout << multiset.Size() << "\n";
    multiset.Remove(2);
    std::cout << multiset.Size() << "\n";
    multiset.Remove(2);
    std::cout << multiset.Size() << "\n";
    multiset.Remove(4);
    multiset.Remove(4);
    multiset.Remove(4);
    multiset.Remove(5);
    multiset.Remove(6);
    multiset.Remove(6);
    multiset.Remove(6);
    multiset.Remove(1);
    multiset.Remove(1);
    multiset.Remove(5);
    multiset.Remove(8);
    multiset.Remove(9);
    std::cout << multiset.Size() << "\n";
    multiset.Print();
    std::cout << "\n";*/
    //
    ////multiset.KOrderStatistic(1);
    //for (int i = 0; i < 14; ++i) {
    //    std::cout << multiset.KOrderStatistic(i) << " ";
    //}
    //std::cout << "\n" << multiset.KOrderStatistic(1);
}
