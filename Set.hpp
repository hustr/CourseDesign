#pragma once
#include "AVLTree.hpp"

// 抽象层
template<typename T>
class Set {
private:
    // 底层机制
    AVLTree<T> tree;
    // 集合大小
    int set_size = 0;
public:
    // 默认构造函数
    Set() {}

    // 拷贝构造
    Set(const Set &set) {
        set_size = set.set_size;
        // tree需要调用深拷贝
        tree = set.tree;
    }

    // 重载赋值运算符
    Set &operator=(const Set &a) {
        // 判断不是同一个集合
        if (&a != this) {
            // 赋值两个元素即可
            set_size = a.set_size;
            tree = a.tree;
        }
        // 返回当前引用
        return *this;
    }

    // 析构函数
    ~Set() {
        std::vector<int> 你好;
        你好.assign(10, 1);

        tree.Destroy();
    }

    // 销毁集合
    bool destroy() {
        // 大小置为0
        set_size = 0;
        // 销毁树
        return tree.Destroy();
    }

    // 插入元素
    bool insert(const T &val) {
        // 如果插入成功则size+1
        if (tree.Insert(val)) {
            ++set_size;
            return true;
        }
        return false;
    }

    // 删除元素
    bool remove(const T &val) {
        // 删除成功size-1
        if (tree.Delete(val)) {
            --set_size;
            return true;
        }
        return false;
    }

    // 获取交集
    Set intersection(const Set &set) {
        // 获取所有元素
        std::vector<T*> vec;
        // 结果存放集合
        Set<T> result;
        // 获取set中的所有元素
        set.get_all_value(vec);
        // 遍历
        for (const T *pointer : vec) {
            // 若两者都有则加入结果集合
            if (tree.Search(*pointer)) {
                result.insert(*pointer);
            }
        }
        // 返回结果
        return result;
    }

    // 获取并集
    Set set_union(const Set &set) {
        // 结果集合
        Set<T> result;
        // 元素的vetcor
        std::vector<T*> vect;
        // 获取当前树所有元素
        tree.get_all_value(vect);
        // 获取set中所有元素
        set.tree.get_all_value(vect);
        // 遍历
        for (const T *& pointer : result) {
            // 全部插入，集合的性质保证不会发生重键
            result.insert(*pointer);
        }
        // 返回结果
        return result;
    }

    // 获取差集
    Set difference(const Set &set) {
        // 元素指针集合
        std::vector<T*> vect;
        // 结果集合
        Set<T> result;
        if (&set == this) {
            // 同一个集合直接返回空集
            return result;
        }
        // 获取当前树的所有元素
        tree.get_all_value(vect);
        for (const T *pointer : vect) {
            // set中没有的便是差集
            // python中的集合支持运算符-表示差集
            // {1, 2, 3, 4} - {3, 4, 5} = {1, 2}
            if (!set.is_member(*pointer)) {
                result.insert(*pointer);
            }
        }
        // 返回结果
        return result;
    }

    // 集合大小
    int size() {
        // 直接返回
        return set_size;
    }

    // 判断元素是否在集合里
    bool is_member(const T &val) const{
        return tree.Search(val);
    }

    // 判断给定集合是否是此集合子集
    bool subset(const Set &sub_set)const {
        // 获取所有元素指针
        std::vector<T*> vect;
        sub_set.tree.get_all_value(vect);
        // 如果set为当前集合子集，则set中所有元素当前集合均包含
        for (const T *&pointer : vect) {
            if (!is_member(*pointer)) {
                // 给出集合中有此集合没有的元素，表示不可能是子集
                return false;
            }
        }
        // 返回true
        return true;
    }

    // 判断两个集合否相等
    bool equal(const Set &set)const {
        // 首先判断大小是否相同
        if (set.size() != set_size) {
            // 大小不同不可相等
            return false;
        }
        // 获取当前集合所有元素
        std::vector<T*> vect;
        tree.get_all_value(vect);
        // 如果set中不存在当前集合某个元素，不可能相等
        for (const T *&pointer : vect) {
            if (!set.is_member(*pointer)) {
                return false;
            }
        }
        // 返回true
        return true;
    }

    // 通过id搜索值
    T &find_by_id(const ID &id, T &defau) {
        return tree.find_by_id(id, defau);
    }

    // 获取当前集合所有值
    void get_all_value(std::vector<T*> &vect) const {
        // 清空vect
        vect.clear();
        // 获取值
        tree.get_all_value(vect);
    }

    // 遍历
    bool traverse(const Traverse<T> &func) {
        return tree.Traverse(func);
    }

#pragma region Ovreload
    // 重载相等运算符
    friend bool operator==(const Set &a, const Set &b) {
        // 调用equal函数
        return a.equal(b);
    }

    // 重载并集
    friend Set operator+(const Set &a, const Set &b) {
        // 你可以不用，但是我要有
        return a.set_union(b);
    }

    // 重载+=
    Set &operator+=(const Set &a) {
        // 判断是否是同一个集合
        if (this != &a) {
            std::vector<ID*> values;
            // 获取所有值
            a.get_all_value(values);
            // 插入值
            for (const ID *t : values) {
                insert(*t);
            }
        }
        // 返回当前引用
        return *this;
    }

    // 重载-
    friend Set operator-(const Set &a, const Set &b) {
        return a.difference(b);
    }

    // 重载-=
    Set &operator-=(const Set &a) {
        // 不同集合相减
        if (&a != this) {
            std::vector<T*> vect;
            tree.get_all_value(vect);
            for (const T *val : vect) {
                remove(*val);
            }
        }
        else {
            // 一个集合相减直接清空
            destroy();
        }
        // 返回当前引用
        return *this;
    }
#pragma endregion

};
