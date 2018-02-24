#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <string>

template<typename T>
using Traverse = void(*)(const T &val);
// ʹ��ID��ʾ�ˣ��洢�����Ƚ�С
using ID = long;
// ��ײ�
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
    // ����
    Node *head = nullptr;
    // ����ʧ�ܵķ���ֵ����-1
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

    //  ������ת����
    void l_rotate(Node *&node) {
        if (node != nullptr) {
            Node *right = node->right;
            node->right = right->left;
            right->left = node;
            // ����node�߶�
            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
            // ����right�߶�
            right->height = std::max(get_height(right->left), get_height(right->right)) + 1;
            // node����
            node = right;
        }
    }

    // ������ת����
    void r_rotate(Node *&node) {
        if (node != nullptr) {
            Node *left = node->left;
            node->left = left->right;
            left->right = node;
            // node�߶�
            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
            // left�߶�
            left->height = std::max(get_height(left->left), get_height(left->right)) + 1;
            // ����
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
        // û�в��벻������ת
        if (!inserted) {
            return false;
        }

        // ��ȡ�ø߶�
        int left_height = get_height(node->left);
        int right_height = get_height(node->right);
        node->height = std::max(left_height, right_height) + 1;

        if (left_height - right_height == 2) {
            // ��߱��ұ߸�2�����뵽�����
            // ��������һ�μ���
            if (val < node->left->value) {
                r_rotate(node);
            }
            else {
                l_rotate(node->left);
                r_rotate(node);
            }
        }
        else if (right_height - left_height == 2) {
            // �Ҳ������2
            if (val > node->right->value) {
                l_rotate(node);
            }
            else {
                r_rotate(node->right);
                l_rotate(node);
            }
        }
        // ��ת����
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

    // ɾ������
    bool _delete(Node *&node, const T &val) {
        // ��ָ�뷵��ɾ��ʧ��
        if (node == nullptr) {
            return false;
        }
        // ����Ƿ�ɾ���ɹ�
        bool deleted = false;
        if (node->value == val) {
            // ���Ҳ�Ϊ�գ�Ҷ�ӽڵ����
            if (node->left == nullptr && node->right == nullptr) {
                // ֱ��ɾ��
                delete(node);
                node = nullptr;
            }
            else if (node->right == nullptr) {
                // �Ҳ�Ϊ�գ���಻Ϊ�գ�ֱ�ӽ����Ľ�����
                Node *left = node->left;
                delete(node);
                // left����Ҫ�����߶�
                node = left;
            }
            else if (node->left == nullptr) {
                // �Ҳ಻Ϊ�գ����Ϊ�����
                Node *right = node->right;
                delete(node);
                // ����Ҫ�����߶�
                node = right;
            }
            else {
                // ���Ҷ���Ϊ��
                // Ѱ�����ӽڵ����ҽ��
                Node *pri = node->left;
                Node *swap = pri->right;
                // ���left�Ҳ�ֱ��Ϊ��ֱ�ӽ���
                if (swap == nullptr) {
                    Node *temp = node;
                    pri->right = node->right;
                    node = pri;
                    delete(temp);
                    // ֱ�ӽ���ȥʱ����߶�
                    int left_h = get_height(node->left);
                    int right_h = get_height(node->right);
                    node->height = std::max(left_h, right_h) + 1;

                    if (right_h - left_h == 2) {
                        // �鿴�Ҳ��Ƿ������ת
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
                    // ʹ��ջ���洢��㣬�ָ���ȷ�߶�
                    std::stack<Node*> nodes;
                    nodes.push(pri);
                    while (swap->right != nullptr) {
                        pri = swap;
                        nodes.push(pri);
                        swap = swap->right;
                    }
                    // pri�Ҳ��Ϊswap��࣬�����߶�
                    pri->right = swap->left;
                    swap->left = node->left;
                    swap->right = node->right;
                    delete(node);
                    node = swap;
                    // ��������ĸ߶�
                    while (!nodes.empty()) {
                        Node *n = nodes.top();
                        nodes.pop();
                        int left_height = get_height(n->left);
                        int right_height = get_height(n->right);
                        n->height = std::max(left_height, right_height) + 1;
                    }
                    Node *left = node->left;
                    // ����swap��node�߶�
                    int left_height = get_height(node->left);
                    int right_height = get_height(node->right);
                    node->height = std::max(left_height, right_height) + 1;
                    // �����߶ȣ����µ��ϵ���
                    // pri�п���ʧȥƽ�⣬���µ��ϵ����߶�
                    if (pri != left) {
                        balance_pri(pri, left->right);
                    }
                    // �������һ���п���node->leftʧȥƽ��
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
                    // ��nodeƽ��
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
            // ����˴���ʾɾ���ɹ�
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
        // ɾ���˵Ļ���Ҫ������������ƽ��
        int left_height = get_height(node->left);
        int right_height = get_height(node->right);
        node->height = std::max(left_height, right_height) + 1;

        // ɾ��һ�����߶Ȳ����ܽ���2�����1
        if (left_height - right_height == 2) {
            // �����Ҳ�������߶ȣ�ֻ�������Ҳ�ɾ��
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
            // �Ҳ�ƫ�ߣ�ֻ�������ɾ��
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

    // ����
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
            // ��ȡָ��
            vect.push_back(&node->value);
            get_all(vect, node->right);
        }
    }
    // ��ȡ�߶�
    int get_h(const Node *node) {
        if (node != nullptr) {
            return std::max(get_h(node->left), get_h(node->right)) + 1;
        }

        return 0;
    }

    void display(Node *node) const {
        Node *nt = node;
        // ��δ�ӡ
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

    // ���
    void copy(const Node *src, Node *&dst) {
        if (src != nullptr) {
            dst = new Node(src->value);
            // �߶���Ϣ�ǵø���
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
    // ���
    AVLTree(const AVLTree &tree) {
        // ����������
        copy(tree.head, head);
    }

    // ���ظ�ֵ=
    AVLTree &operator=(const AVLTree &tree) {
        if (&tree != this) {
            // �������
            Destroy();
            // �������������ƹ���
            copy(tree.head, head);
        }
        // ��������
        return *this;
    }

    // ��������
    ~AVLTree() {
        destroy(head);
    }

    // ������
    bool Destroy() {
        return destroy(head);
    }

    // ����Ԫ��
    bool Search(const T &val) const{
        return search(head, val);
    }

    // ����Ԫ��
    bool Insert(const T &val) {
        return insert(head, val);

    }

    // ����Ԫ��
    bool Delete(const T &val) {
        return _delete(head, val);
    }

    // ������
    bool Traverse(const Traverse<T> &func) {
        return  traverse(head, func);
    }

    // ��ȡ����Ԫ�ص�ָ��
    void get_all_value(std::vector<T*> &vec) const {
        vec.clear();
        get_all(vec, head);
    }

    // �ж��Ƿ�ƽ��
    bool is_balance() {
        return is_balance(head);
    }

    T &find_by_id(const ID &id) {
        return find_by_id(head, id);
    }

    // ��ʾ����Ԫ��
    void display() const {
        Node *nt = head;
        // ��δ�ӡ
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
