#pragma once
#include "AVLTree.h"

// �����
template<typename T>
class Set {
private:
    // �ײ����
    AVLTree<T> tree;
    // ���ϴ�С
    int set_size = 0;
public:
    Set() {}
    Set(const Set &set) {
        set_size = set.set_size;
        // tree��Ҫ���
        tree = set.tree;
    }

    Set &operator=(const Set &a) {
        if (&a != this) {
            // ��ֵ����Ԫ�ؼ���
            set_size = a.set_size;
            tree = a.tree;
        }

        return *this;
    }

    ~Set() {
        tree.Destroy();
    }
    // �ݻ�
    bool destroy() {
        set_size = 0;
        return tree.Destroy();
    }

    // ����Ԫ��
    bool insert(const T &val) {
        if (tree.Insert(val)) {
            ++set_size;
            return true;
        }
        return false;
    }

    // ɾ��Ԫ��
    bool remove(const T &val) {
        if (tree.Delete(val)) {
            --set_size;
        }
        return false;
    }

    // ����
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

    // ����
    Set set_union(Set &set) {
        Set<T> result;
        std::vector<T*> vect;
        tree.get_all_value(vect);
        set.tree.get_all_value(vect);
        for (const T *& pointer : result) {
            // ȫ�����룬���ϵ����ʱ�֤���ᷢ���ؼ�
            result.insert(*pointer);
        }

        return result;
    }

    // ���������Ϊ��
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

    // ���ϴ�С
    int size() {
        return set_size;
    }

    // �ж�Ԫ���Ƿ��ڼ�����
    bool is_member(const T &val) const{
        return tree.Search(val);
    }

    // �жϸ��������Ƿ��Ǵ˼����Ӽ�
    bool subset(const Set &sub_set)const {
        std::vector<T*> vect;
        sub_set.tree.get_all_value(vect);
        for (const T *&pointer : vect) {
            if (!is_member(*pointer)) {
                // �����������д˼���û�е�Ԫ�أ���ʾ���������Ӽ�
                return false;
            }
        }

        return true;
    }

    // �ж��������Ϸ����
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
    // �������
    friend std::ostream &operator<<(const std::ostream &os, const Set &set) {

    }

    // ������������
    friend bool operator==(const Set &a, const Set &b) {
        return a.equal(b);
    }

    // ���ز���
    friend Set operator+(const Set &a, const Set &b) {
        return a.set_union(b);
    }

    // ����+=
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
