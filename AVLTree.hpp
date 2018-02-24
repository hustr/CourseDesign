#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <string>

template<typename T>
using Traverse = void(*)(const T &val);
// 使用ID表示人，存储开销比较小
using ID = long;
// 最底层
template<typename T>
class AVLTree {
private:
    struct Node {
        T value;
        Node *left = nullptr;
        Node *right = nullptr;
        int height = 1;

        explicit Node(const T &value) : value(value) {}
    };
    // 树根
    Node *head = nullptr;
    // 查找失败的返回值等于-1
    T none;
    bool destroy(Node *&node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
            node = nullptr;
        }
        return true;
    }

    bool search(Node *node, const T &value)const {
        if (node != nullptr) {
            if (node->value > value) {
                return search(node->left, value);
            }
            else if (node->value < value) {
                return search(node->right, value);
            }
            return true;
        }

        return false;
    }

    inline int get_height(Node *node) {
        return node == nullptr ? 0 : node->height;
    }

    //  向左旋转函数
    void l_rotate(Node *&node) {
        if (node != nullptr) {
            Node *right = node->right;
            node->right = right->left;
            right->left = node;
            // 调整node高度
            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
            // 调整right高度
            right->height = std::max(get_height(right->left), get_height(right->right)) + 1;
            // node交接
            node = right;
        }
    }

    // 向右旋转函数
    void r_rotate(Node *&node) {
        if (node != nullptr) {
            Node *left = node->left;
            node->left = left->right;
            left->right = node;
            // node高度
            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
            // left高度
            left->height = std::max(get_height(left->left), get_height(left->right)) + 1;
            // 交接
            node = left;
        }
    }

    bool insert(Node *&node, const T &val) {
        bool inserted = false;
        if (node == nullptr) {
            node = new Node(val);
            return true;
        }
        else if (node->value > val) {
            inserted = insert(node->left, val);
        }
        else if (node->value < val) {
            inserted = insert(node->right, val);
        }
        // 没有插入不考虑旋转
        if (!inserted) {
            return false;
        }

        // 先取得高度
        int left_height = get_height(node->left);
        int right_height = get_height(node->right);
        node->height = std::max(left_height, right_height) + 1;

        if (left_height - right_height == 2) {
            // 左边比右边高2，插入到了左边
            // 左左，右移一次即可
            if (val < node->left->value) {
                r_rotate(node);
            }
            else {
                l_rotate(node->left);
                r_rotate(node);
            }
        }
        else if (right_height - left_height == 2) {
            // 右侧比左侧高2
            if (val > node->right->value) {
                l_rotate(node);
            }
            else {
                r_rotate(node->right);
                l_rotate(node);
            }
        }
        // 旋转过后
        left_height = get_height(node->left);
        right_height = get_height(node->right);
        node->height = std::max(left_height, right_height) + 1;

        return true;
    }

    void balance_pri(const Node *pri, Node *&node) {
        if (node == pri) {
            int left_h = get_height(node->left);
            int right_h = get_height(node->right);
            node->height = std::max(left_h, right_h) + 1;
            if (left_h - right_h == 2) {
                int l = get_height(node->left->left);
                int r = get_height(node->left->right);
                if (l >= r) {
                    r_rotate(node);
                }
                else {
                    l_rotate(node->left);
                    r_rotate(node);
                }
            }
        }
        else {
            balance_pri(pri, node->right);
            int left_h = get_height(node->left);
            int right_h = get_height(node->right);
            node->height = std::max(left_h, right_h) + 1;
            if (left_h - right_h == 2) {
                int l = get_height(node->left->left);
                int r = get_height(node->left->right);
                if (l >= r) {
                    r_rotate(node);
                }
                else {
                    l_rotate(node->left);
                    r_rotate(node);
                }
            }
        }
        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
    }

    // 删除操作
    bool _delete(Node *&node, const T &val) {
        // 空指针返回删除失败
        if (node == nullptr) {
            return false;
        }
        // 标记是否删除成功
        bool deleted = false;
        if (node->value == val) {
            // 左右侧为空，叶子节点情况
            if (node->left == nullptr && node->right == nullptr) {
                // 直接删除
                delete(node);
                node = nullptr;
            }
            else if (node->right == nullptr) {
                // 右侧为空，左侧不为空，直接将左侧的接上来
                Node *left = node->left;
                delete(node);
                // left不需要调整高度
                node = left;
            }
            else if (node->left == nullptr) {
                // 右侧不为空，左侧为空情况
                Node *right = node->right;
                delete(node);
                // 不需要调整高度
                node = right;
            }
            else {
                // 左右都不为空
                // 寻找左子节点最右结点
                Node *pri = node->left;
                Node *swap = pri->right;
                // 如果left右侧直接为空直接交换
                if (swap == nullptr) {
                    Node *temp = node;
                    pri->right = node->right;
                    node = pri;
                    delete(temp);
                    // 直接接上去时计算高度
                    int left_h = get_height(node->left);
                    int right_h = get_height(node->right);
                    node->height = std::max(left_h, right_h) + 1;

                    if (right_h - left_h == 2) {
                        // 查看右侧是否可以旋转
                        int l = get_height(node->right->left);
                        int r = get_height(node->right->right);
                        if (l <= r) {
                            l_rotate(node);
                        }
                        else {
                            r_rotate(node->right);
                            l_rotate(node);
                        }
                    }

                    left_h = get_height(node->left);
                    right_h = get_height(node->right);
                    node->height = std::max(left_h, right_h) + 1;
                }
                else {
                    // 使用栈来存储结点，恢复正确高度
                    std::stack<Node*> nodes;
                    nodes.push(pri);
                    while (swap->right != nullptr) {
                        pri = swap;
                        nodes.push(pri);
                        swap = swap->right;
                    }
                    // pri右侧变为swap左侧，调整高度
                    pri->right = swap->left;
                    swap->left = node->left;
                    swap->right = node->right;
                    delete(node);
                    node = swap;
                    // 调整下面的高度
                    while (!nodes.empty()) {
                        Node *n = nodes.top();
                        nodes.pop();
                        int left_height = get_height(n->left);
                        int right_height = get_height(n->right);
                        n->height = std::max(left_height, right_height) + 1;
                    }
                    Node *left = node->left;
                    // 调整swap即node高度
                    int left_height = get_height(node->left);
                    int right_height = get_height(node->right);
                    node->height = std::max(left_height, right_height) + 1;
                    // 调整高度，从下到上调整
                    // pri有可能失去平衡，从下到上调整高度
                    if (pri != left) {
                        balance_pri(pri, left->right);
                    }
                    // 左侧少了一个有可能node->left失去平衡
                    left_height = get_height(left->left);
                    right_height = get_height(left->right);
                    left->height = std::max(left_height, right_height) + 1;
                    if (left_height - right_height == 2) {
                        int l = get_height(left->left->left);
                        int r = get_height(left->left->right);
                        if (l >= r) {
                            r_rotate(node->left);
                        }
                        else {
                            l_rotate(left->left);
                            r_rotate(node->left);
                        }
                    }
                    left_height = get_height(node->left);
                    right_height = get_height(node->right);
                    node->height = std::max(left_height, right_height) + 1;
                    // 将node平衡
                    if (right_height - left_height == 2) {
                        int l = get_height(node->right->left);
                        int r = get_height(node->right->right);
                        if (l <= r) {
                            l_rotate(node);
                        }
                        else {
                            r_rotate(node->right);
                            l_rotate(node);
                        }
                    }
                    left_height = get_height(node->left);
                    right_height = get_height(node->right);
                    node->height = std::max(left_height, right_height) + 1;
                }
            }
            // 到达此处表示删除成功
            return true;
        }
        else if (node->value > val) {
            deleted = _delete(node->left, val);
        }
        else {
            deleted = _delete(node->right, val);
        }

        if (!deleted) {
            return false;
        }
        // 删除了的话需要调整保持树的平衡
        int left_height = get_height(node->left);
        int right_height = get_height(node->right);
        node->height = std::max(left_height, right_height) + 1;

        // 删除一个结点高度不可能降低2，最多1
        if (left_height - right_height == 2) {
            // 左侧比右侧高两个高度，只可能是右侧删除
            Node *left = node->left;
            int left_h = get_height(left->left);
            int right_h = get_height(left->right);
            if (left_h >= right_h) {
                r_rotate(node);
            }
            else {
                l_rotate(node->left);
                r_rotate(node);
            }
        }
        else if (right_height - left_height == 2) {
            // 右侧偏高，只能是左侧删除
            Node *right = node->right;
            int left_h = get_height(right->left);
            int right_h = get_height(right->right);
            if (left_h <= right_h) {
                l_rotate(node);
            }
            else {
                r_rotate(node->right);
                l_rotate(node);
            }
        }
        left_height = get_height(node->left);
        right_height = get_height(node->right);
        node->height = std::max(left_height, right_height) + 1;

        return true;
    }

    // 遍历
    void traverse(const Node *node, const Traverse<T> &func) {
        if (node != nullptr) {
            traverse(node->left, func);
            func(node->value);
            traverse(node->right, func);
        }
    }

    void get_all(std::vector<T*> &vect, Node *node)const {
        if (node != nullptr) {
            get_all(vect, node->left);
            // 获取指针
            vect.push_back(&node->value);
            get_all(vect, node->right);
        }
    }
    // 获取高度
    int get_h(const Node *node) {
        if (node != nullptr) {
            return std::max(get_h(node->left), get_h(node->right)) + 1;
        }

        return 0;
    }

    void display(Node *node) const {
        Node *nt = node;
        // 层次打印
        std::vector<Node*> *nodes1 = new std::vector<Node*>();
        std::vector<Node*> *nodes2 = new std::vector<Node*>();
        if (head != nullptr) {
            nodes1->push_back(nt);
            while (!nodes1->empty()) {
                for (size_t i = 0; i < nodes1->size(); i++) {
                    Node *n = nodes1->at(i);
                    std::cout << n->value << ", ";
                    if (n->left != nullptr) {
                        nodes2->push_back(n->left);
                    }
                    if (n->right != nullptr) {
                        nodes2->push_back(n->right);
                    }
                }
                std::cout << "\n";
                nodes1->clear();
                std::swap(nodes1, nodes2);
            }
        }
        std::cout << "\n";

        delete(nodes1);
        delete(nodes2);
    }

    bool is_balance(Node *node) {
        if (node != nullptr) {
            if (!is_balance(node->left)) {
                /*            std::cout << "In avl:\n";
                            display(node->left);*/
                return false;
            }
            if (!is_balance(node->right)) {
                //std::cout << "In avl:\n";
                //display(node->right);
                return false;
            }
            int left_h = get_h(node->left);
            int right_h = get_h(node->right);
            return std::abs(left_h - right_h) <= 1;
        }

        return true;
    }

    // 深拷贝
    void copy(const Node *src, Node *&dst) {
        if (src != nullptr) {
            dst = new Node(src->value);
            // 高度信息记得复制
            dst->height = src->height;
            copy(src->left, dst->left);
            copy(src->right, dst->right);
        }
    }

    T &find_by_id(Node *&node, const ID &id) {
        if (node != nullptr) {
            if (node->value > id) {
                return find_by_id(node->left, id);
            }
            else if (node->value < id) {
                return find_by_id(node->right, id);
            }
            return node->value;
        }
        return none;
    }
public:
    AVLTree(){}
    // 深拷贝
    AVLTree(const AVLTree &tree) {
        // 拷贝整棵树
        copy(tree.head, head);
    }

    // 重载赋值=
    AVLTree &operator=(const AVLTree &tree) {
        if (&tree != this) {
            // 将树清空
            Destroy();
            // 将参数的树复制过来
            copy(tree.head, head);
        }
        // 返回引用
        return *this;
    }

    // 析构函数
    ~AVLTree() {
        destroy(head);
    }

    // 销毁树
    bool Destroy() {
        return destroy(head);
    }

    // 搜索元素
    bool Search(const T &val) const{
        return search(head, val);
    }

    // 插入元素
    bool Insert(const T &val) {
        return insert(head, val);

    }

    // 插入元素
    bool Delete(const T &val) {
        return _delete(head, val);
    }

    // 遍历树
    bool Traverse(const Traverse<T> &func) {
        return  traverse(head, func);
    }

    // 获取所有元素的指针
    void get_all_value(std::vector<T*> &vec) const {
        vec.clear();
        get_all(vec, head);
    }

    // 判断是否平衡
    bool is_balance() {
        return is_balance(head);
    }

    T &find_by_id(const ID &id) {
        return find_by_id(head, id);
    }

    // 显示所有元素
    void display() const {
        Node *nt = head;
        // 层次打印
        std::vector<Node*> *nodes1 = new std::vector<Node*>();
        std::vector<Node*> *nodes2 = new std::vector<Node*>();
        if (head != nullptr) {
            nodes1->push_back(nt);
            while (!nodes1->empty()) {
                for (size_t i = 0; i < nodes1->size(); i++) {
                    Node *n = nodes1->at(i);
                    std::cout << n->value << ", ";
                    if (n->left != nullptr) {
                        nodes2->push_back(n->left);
                    }
                    if (n->right != nullptr) {
                        nodes2->push_back(n->right);
                    }
                }
                std::cout << "\n";
                nodes1->clear();
                std::swap(nodes1, nodes2);
            }
        }
        std::cout << "\n";

        delete(nodes1);
        delete(nodes2);
    }
};
