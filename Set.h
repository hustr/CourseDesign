#pragma once
#include "AVLTree.h"

// 抽象层
template<typename T>
class Set {
private:
    // 底层机制
    AVLTree<T> tree;
    // 集合大小
    int set_size = 0;
public:
    Set() {}
    Set(const Set &set) {
        set_size = set.set_size;
        // tree需要深拷贝
        tree = set.tree;
    }

    Set &operator=(const Set &a) {
        if (&a != this) {
            // 赋值两个元素即可
            set_size = a.set_size;
            tree = a.tree;
        }

        return *this;
    }

    ~Set() {
        tree.Destroy();
    }
    // 摧毁
    bool destroy() {
        set_size = 0;
        return tree.Destroy();
    }

    // 插入元素
    bool insert(const T &val) {
        if (tree.Insert(val)) {
            ++set_size;
            return true;
        }
        return false;
    }

    // 删除元素
    bool remove(const T &val) {
        if (tree.Delete(val)) {
            --set_size;
        }
        return false;
    }

    // 交集
    Set intersection(const Set &set) {
        std::vector<T*> vec;
        Set<T> result;
        set.get_all_value(vec);
        for (const T *pointer : vec) {
            if (tree.Search(*pointer)) {
                result.insert(*pointer);
            }
        }

        return result;
    }

    // 并集
    Set set_union(Set &set) {
        Set<T> result;
        std::vector<T*> vect;
        tree.get_all_value(vect);
        set.tree.get_all_value(vect);
        for (const T *& pointer : result) {
            // 全部插入，集合的性质保证不会发生重键
            result.insert(*pointer);
        }

        return result;
    }

    // 差集，暂且认为是
    Set difference(const Set &set) {
        std::vector<T*> vect;
        Set<T> result;
        tree.get_all_value(vect);
        for (const T *pointer : vect) {
            if (!set.is_member(*pointer)) {
                result.insert(*pointer);
            }
        }

        return result;
    }

    // 集合大小
    int size() {
        return set_size;
    }

    // 判断元素是否在集合里
    bool is_member(const T &val) const{
        return tree.Search(val);
    }

    // 判断给定集合是否是此集合子集
    bool subset(const Set &sub_set)const {
        std::vector<T*> vect;
        sub_set.tree.get_all_value(vect);
        for (const T *&pointer : vect) {
            if (!is_member(*pointer)) {
                // 给出集合中有此集合没有的元素，表示不可能是子集
                return false;
            }
        }

        return true;
    }

    // 判断两个集合否相等
    bool equal(const Set &set)const {

        if (set.size() != set_size) {
            return false;
        }
        std::vector<T*> vect;
        tree.get_all_value(vect);
        for (const T *&pointer : vect) {
            if (!set.is_member(*pointer)) {
                return false;
            }
        }

        return true;
    }

    T &find_by_id(const ID &id) {
        return tree.find_by_id(id);
    }

    void get_all_value(std::vector<T*> &vect) const {
        vect.clear();
        tree.get_all_value(vect);
    }
#pragma region Ovreload
    // 重载输出
    friend std::ostream &operator<<(const std::ostream &os, const Set &set) {

    }

    // 重载相等运算符
    friend bool operator==(const Set &a, const Set &b) {
        return a.equal(b);
    }

    // 重载并集
    friend Set operator+(const Set &a, const Set &b) {
        return a.set_union(b);
    }

    // 重载+=
    Set &operator+=(const Set a) {
        std::vector<ID*> values;
        a.get_all_value(values);
        for (const ID *t : values) {
            insert(*t);
        }

        return *this;
    }
#pragma endregion

};
