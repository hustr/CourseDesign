#pragma once

#include "Set.hpp"

// 使用int标识喜好
using Hobby = std::string;
// 使用string表示名称
using Name = std::string;

// Peorson类型定义，不需要使用泛型了
class Person {
private:
    // 唯一标识id
    ID id = -1;
    // 名称
    Name name = "-1";
    // 喜好，使用string模拟
    Set<Hobby> hobbies;
    // 关注的人
    Set<ID> watches;
    // 粉丝集
    Set<ID> fans;
public:
    // 默认构造函数
    Person(){}
    // 含参构造函数
    Person(const ID &id, const Name &name):id(id), name(name){}

    // 获取人的id
    ID get_id() const {
        return id;
    }

    // 获取名称
    Name get_name()const {
        return name;
    }

    // 获取一个人的喜好
    Set<Hobby> get_hobbies() const {
        return hobbies;
    }

    // 获取其关注的人
    Set<ID> get_watches()const {
        return watches;
    }

    // 获取粉丝集
    Set<ID> get_fans() const{
        return fans;
    }

    // 取消关注
    bool cancel_watch(const ID &other_person) {
        // 尝试删除并返回删除结果
        return watches.remove(other_person);
    }

    // 关注某人
    bool add_watch(const ID &other_person) {
        // 不允许关注自己
        if (other_person != id) {
            return watches.insert(other_person);
        }
        // 尝试关注自己，返回错误
        return false;
    }

    // 移除粉丝
    bool remove_fans(const ID &other_person) {
        return fans.remove(other_person);
    }

    // 添加粉丝，仅在初始化时使用
    bool add_funs(const ID &other_person) {
        if (other_person != id) {
            return fans.insert(other_person);
        }
        return false;
    }

    // 添加爱好
    bool add_hobby(const Hobby &like) {
        // 尝试加入并返回添加结果
        return hobbies.insert(like);
    }

    // 删除爱好
    bool remove_hobby(const Hobby &like) {
        return hobbies.remove(like);
    }

#pragma region Ovreload
    // 重载<运算符
    friend bool operator<(const Person &a, const Person &b) {
        return a.id < b.id;
    }
    friend bool operator<(const Person &a, const ID &a_id) {
        return a.id < a_id;
    }
    friend bool operator<(const ID &a_id, const Person &a) {
        return a_id < a.id;
    }

    // 重载>运算符
    friend bool operator>(const Person &a, const Person &b) {
        return a.id > b.id;
    }
    friend bool operator>(const Person &a, const ID &a_id) {
        return a.id > a_id;
    }
    friend bool operator>(const ID &a_id, const Person &a) {
        return a_id > a.id;
    }

    // 重载==运算符
    friend bool operator==(const Person &a, const Person &b) {
        return a.id == b.id;
    }
    friend bool operator==(const Person &a, const ID &a_id) {
        return a.id == a_id;
    }
    friend bool operator==(const ID &a_id, const Person &a) {
        return a_id == a.id;
    }

    // 重载!=运算符
    friend bool operator!=(const Person &a, const Person &b) {
        return a.id != b.id;
    }

    // 重载输出运算符
    friend std::ostream &operator<<(std::ostream &os, const Person &a) {
        os << "ID: " << a.id << ", name: " << a.name;
        return os;
    }
#pragma endregion

};
