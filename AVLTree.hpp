#pragma once
// 包含头文件
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <string>

// 使用泛型，便于代码重用
template<typename T>
using Traverse = void(*)(const T &val);
// 使用ID表示人，存储开销比较小
using ID = long;
// 最底层
template<typename T>
class AVLTree {
private:
    // 节点定义
    struct Node {
        // 值域
        T value;
        // 左孩子节点
        Node *left = nullptr;
        // 右孩子节点
        Node *right = nullptr;
        // 高度
        int height = 1;
        //删除默认构造函数
        Node() = delete;
        // 唯一构造函数
        explicit Node(const T &value) : value(value) {}
    };

    // 树根
    Node *head = nullptr;

    // 销毁
    bool destroy(Node *&node) {
        if (node != nullptr) {
            // 递归销毁左右子节点，再销毁此节点
            destroy(node->left);
            destroy(node->right);
            delete node;
            // 节点指向置为nullptr
            node = nullptr;
        }
        // 返回销毁结果
        return true;
    }

    // 搜索函数
    bool search(Node *node, const T &value)const {
        // 如果参数不为空
        if (node != nullptr) {
            // 简单的递归二叉搜索树算法
            if (node->value > value) {
                return search(node->left, value);
            }
            else if (node->value < value) {
                return search(node->right, value);
            }
            return true;
        }
        // 为空表示搜索失败，返回false
        return false;
    }

    // 获取节点高度
    inline int get_height(Node *node) {
        // 空指针返回0
        return node == nullptr ? 0 : node->height;
    }

    //  向左旋转函数
    void l_rotate(Node *&node) {
        // 节点不为空再进行操作
        if (node != nullptr) {
            // 旋转
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
            // 旋转
            Node *left = node->left;
            node->left = left->right;
            left->right = node;
            // node高度
            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
            // left高度
            left->height = std::max(get_height(left->left), get_height(left->right)) + 1;
            // 更改node的值
            node = left;
        }
    }

    // 插入函数
    bool insert(Node *&node, const T &val) {
        // 标记是否插入成功
        bool inserted = false;
        if (node == nullptr) {
            // 空节点表示可以直接插入
            node = new Node(val);
            // 返回true
            return true;
        }
        else if (node->value > val) {
            // 递归插入
            inserted = insert(node->left, val);
        }
        else if (node->value < val) {
            // 递归插入
            inserted = insert(node->right, val);
        }
        // 没有插入不考虑旋转
        if (!inserted) {
            return false;
        }

        // 先取得高度
        int left_height = get_height(node->left);
        int right_height = get_height(node->right);
        // node高度更新
        node->height = std::max(left_height, right_height) + 1;
        // 树的高度差过大，失去平衡
        if (left_height - right_height == 2) {
            // 左边比右边高2，插入到了左边
            // 左左，右移一次即可
            if (val < node->left->value) {
                r_rotate(node);
            }
            else {
                // 左右情况
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
                // 右左旋转即可
                r_rotate(node->right);
                l_rotate(node);
            }
        }
        //// 旋转过后，插入时保证了插入前的高度正确则插入后的一定正确
        //left_height = get_height(node->left);
        //right_height = get_height(node->right);
        //// 
        //node->height = std::max(left_height, right_height) + 1;

        return true;
    }

    // 删除时若节点左右都不为空时调整pri节点
    void balance_pri(const Node *pri, Node *&node) {
        // 我在删除的时候采用的时将左节点最右节点替换过来
        // ，只会导致pri右侧变低，因此pri前面的节点均需要调整，
        // 递归终结在pri处
        if (node == pri) {
            int left_h = get_height(node->left);
            int right_h = get_height(node->right);
            node->height = std::max(left_h, right_h) + 1;
            // 判断右侧是否过低
            if (left_h - right_h == 2) {
                int l = get_height(node->left->left);
                int r = get_height(node->left->right);
                // 判断pri左侧是否比右侧低，由于现在是需要右旋，
                // 需要考虑类似插入时的左右情况
                if (l >= r) {
                    r_rotate(node);
                }
                else {
                    // 左右情况需要先调整左节点的高度
                    l_rotate(node->left);
                    // 右旋
                    r_rotate(node);
                }
            }
        }
        else {
            //  递归，由下到上
            balance_pri(pri, node->right);
            // 调整类似上面的
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
                    // 将left直接接上来，可能导致新node左侧过低
                    // 判断并调整平衡
                    Node *temp = node;
                    pri->right = node->right;
                    node = pri;
                    delete(temp);
                    // 直接接上去时计算高度
                    int left_h = get_height(node->left);
                    int right_h = get_height(node->right);
                    node->height = std::max(left_h, right_h) + 1;
                    // 判断左侧是否过低
                    if (right_h - left_h == 2) {
                        // 查看右侧是否可以旋转
                        int l = get_height(node->right->left);
                        int r = get_height(node->right->right);
                        // 判断是否是类似插入右左情况
                        if (l <= r) {
                            l_rotate(node);
                        }
                        else {
                            // 是右左情况先调整right节点
                            r_rotate(node->right);
                            l_rotate(node);
                        }
                    }

                    //left_h = get_height(node->left);
                    //right_h = get_height(node->right);
                    //node->height = std::max(left_h, right_h) + 1;
                }
                else {
                    // 使用栈来存储结点，恢复一串pri正确高度
                    std::stack<Node*> nodes;
                    // 压入left节点，不知道有没有用
                    //nodes.push(pri);
                    while (swap->right != nullptr) {
                        // 寻找left节点最右节点
                        pri = swap;
                        //  pri记得压入栈
                        nodes.push(pri);
                        // swap继续向右
                        swap = swap->right;
                    }
                    // pri右侧变为swap左侧，调整高度
                    pri->right = swap->left;
                    swap->left = node->left;
                    swap->right = node->right;
                    // 释放空间
                    delete(node);
                    node = swap;
                    // 调整一堆pri的高度，便于接下来balance_pri操作
                    while (!nodes.empty()) {
                        Node *n = nodes.top();
                        // 指针弹出
                        nodes.pop();
                        int left_height = get_height(n->left);
                        int right_height = get_height(n->right);
                        // 获取最大高度加一即为当前高度
                        n->height = std::max(left_height, right_height) + 1;
                    }
                    //// 调整交换后swap即node高度
                    //int left_height = get_height(node->left);
                    //int right_height = get_height(node->right);
                    //node->height = std::max(left_height, right_height) + 1;
                    // 调整高度，从下到上调整
                    // pri有可能失去平衡，从下到上调整高度
                    Node *left = node->left;
                    // 进行递归调整
                    if (pri != left) {
                        balance_pri(pri, left->right);
                    }
                    // 左侧少了一个有可能node->left失去平衡
                    int left_height = get_height(left->left);
                    int right_height = get_height(left->right);
                    left->height = std::max(left_height, right_height) + 1;
                    // 查看右侧是否过低
                    if (left_height - right_height == 2) {
                        int l = get_height(left->left->left);
                        int r = get_height(left->left->right);
                        // 判断是否是左右情况
                        if (l >= r) {
                            r_rotate(node->left);
                        }
                        else {
                            // 左右情况先左旋再右旋
                            l_rotate(left->left);
                            r_rotate(node->left);
                        }
                    }
                    // 下面的都调整完后调整node节点高度
                    left_height = get_height(node->left);
                    right_height = get_height(node->right);
                    node->height = std::max(left_height, right_height) + 1;
                    // node左侧减少一个节点，判断左侧是否过低
                    if (right_height - left_height == 2) {
                        int l = get_height(node->right->left);
                        int r = get_height(node->right->right);
                        // 判断是否是右左情况
                        if (l <= r) {
                            l_rotate(node);
                        }
                        else {
                            // 右左情况需要先右旋再左旋
                            r_rotate(node->right);
                            l_rotate(node);
                        }
                    }
             /*       left_height = get_height(node->left);
                    right_height = get_height(node->right);
                    node->height = std::max(left_height, right_height) + 1;*/
                }
            }
            // 到达此处表示删除成功
            return true;
        }
        else if (node->value > val) {
            // 递归删除
            deleted = _delete(node->left, val);
        }
        else {
            // 递归删除
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
            // 判断是否是左右情况
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
            // 判断是否是右左情况
            if (left_h <= right_h) {
                l_rotate(node);
            }
            else {
                r_rotate(node->right);
                l_rotate(node);
            }
        }
        //left_height = get_height(node->left);
        //right_height = get_height(node->right);
        //node->height = std::max(left_height, right_height) + 1;

        return true;
    }

    // 遍历
    void traverse(const Node *node, const Traverse<T> &func) {
        if (node != nullptr) {
            // 中序遍历
            traverse(node->left, func);
            func(node->value);
            traverse(node->right, func);
        }
    }

    // 获取所有的节点的指针
    void get_all(std::vector<T*> &vect, Node *node)const {
        if (node != nullptr) {
            // 中序遍历获取
            get_all(vect, node->left);
            // 获取指针
            vect.push_back(&node->value);
            get_all(vect, node->right);
        }
    }

    // 获取高度，效率极低，仅在测试时判断树是否平衡使用
    int get_h(const Node *node) {
        if (node != nullptr) {
            // 获取node高度
            return std::max(get_h(node->left), get_h(node->right)) + 1;
        }
        // 空节点高度返回0
        return 0;
    }

    void display(Node *node) {
        //Node *nt = node;
        // 层次打印
        std::vector<Node*> nodes1, nodes2;
        if (node != nullptr) {
            nodes1.push_back(node);
            while (!nodes1.empty()) {
                for (size_t i = 0; i < nodes1.size(); i++) {
                    Node *n = nodes1[i];
                    std::cout << n->value << ", ";
                    if (n->left != nullptr) {
                        nodes2.push_back(n->left);
                    }
                    if (n->right != nullptr) {
                        nodes2.push_back(n->right);
                    }
                }
                std::cout << "\n";
                // nodes1清空
                nodes1.clear();
                // 交换nodes1和nodes2
                nodes1.swap(nodes2);
                //std::swap(nodes1, nodes2);
            }
        }
        std::cout << "\n";
    }

    // 效率极低，仅在测试时检测树是否平衡使用
    bool is_balance(Node *node) {
        if (node != nullptr) {
            if (!is_balance(node->left)) {
                /*            std::cout << "In avl:\n";
                            display(node->left);*/
                // 不平衡返回false
                return false;
            }
            if (!is_balance(node->right)) {
                //std::cout << "In avl:\n";
                //display(node->right);
                return false;
            }
            int left_h = get_h(node->left);
            int right_h = get_h(node->right);
            // 左右高度差最多相差1即平衡
            return std::abs(left_h - right_h) <= 1;
        }
        // 空节点默认平衡
        return true;
    }

    // 对树进行深拷贝
    void deep_copy(const Node *src, Node *&dst) {
        if (src != nullptr) {
            // 深拷贝，不多说
            dst = new Node(src->value);
            // 高度信息记得复制
            dst->height = src->height;
            //  递归复制
            deep_copy(src->left, dst->left);
            deep_copy(src->right, dst->right);
        }
    }

    // 根据id寻找节点
    T &find_by_id(Node *&node, const ID &id, T &defau) {
        if (node != nullptr) {
            // 同搜索
            if (node->value > id) {
                return find_by_id(node->left, id, defau);
            }
            else if (node->value < id) {
                return find_by_id(node->right, id, defau);
            }
            return node->value;
        }
        // 搜索失败返回默认值默认值
        return defau;
    }
public:
    // 默认构造函数
    AVLTree(){}
    // 构造函数
    AVLTree(const AVLTree &tree) {
        // 拷贝整棵树
        deep_copy(tree.head, head);
    }

    // 重载赋值=
    AVLTree &operator=(const AVLTree &tree) {
        // 判断是否为同一棵树
        if (&tree != this) {
            // 将树清空
            Destroy();
            // 将参数的树复制过来
            deep_copy(tree.head, head);
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

    // 删除元素
    bool Delete(const T &val) {
        return _delete(head, val);
    }

    // 遍历树
    bool Traverse(const Traverse<T> &func) {
        return  traverse(head, func);
    }

    // 获取所有元素的指针
    void get_all_value(std::vector<T*> &vec) const {
        // 先对vec清空
        vec.clear();
        get_all(vec, head);
    }

    // 判断是否平衡
    bool is_balance() {
        return is_balance(head);
    }

    // 通过id搜索值的引用
    T &find_by_id(const ID &id, T &defau) {
        return find_by_id(head, id, defau);
    }

    // 显示所有元素
    void display() const {
        display(head);
    }
};
