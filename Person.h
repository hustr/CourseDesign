#pragma once

#include "Set.h"

// ʹ��int��ʶϲ��
using Hobby = std::string;
// ʹ��string��ʾ����
using Name = std::string;

class Person {
private:
    // Ψһ��ʶid
    ID id = -1;
    // ����
    Name name = "-1";
    // ϲ�ã�ʹ��stringģ��
    Set<Hobby> hobbies;
    // ��ע����
    Set<ID> watches;
    // ��˿��
    Set<ID> fans;
public:
    // Ĭ�Ϲ��캯��
    Person(){}
    // ���ι��캯��
    Person(const ID &id, const Name &name):id(id), name(name){}

    // ��ȡ�˵�id
    ID get_id() const {
        return id;
    }

    // ����id
    void set_id(const ID &id) {
        this->id = id;
    }

    // ��ȡ����
    Name get_name()const {
        return name;
    }

    // ��ȡһ���˵�ϲ��
    Set<Hobby> get_hobbies() const {
        return hobbies;
    }

    // ��ȡ���ע����
    Set<ID> get_watches()const {
        return watches;
    }

    // ��ȡ��˿��
    Set<ID> get_fans() const{
        return fans;
    }

    // ȡ����ע
    bool cancel_watch(const ID &other_person) {
        // ����ɾ��������ɾ�����
        return watches.remove(other_person);
    }

    // ��עĳ��
    bool add_watch(const ID &other_person) {
        // �������ע�Լ�
        if (other_person != id) {
            return watches.insert(other_person);
        }
        // ���Թ�ע�Լ������ش���
        return false;
    }

    // �Ƴ���˿
    bool remove_fans(const ID &other_person) {
        return fans.remove(other_person);
    }

    // ��ӷ�˿�����ڳ�ʼ��ʱʹ��
    bool add_funs(const ID &other_person) {
        if (other_person != id) {
            return fans.insert(other_person);
        }
        return false;
    }

    // ��Ӱ���
    bool add_hobby(const Hobby &like) {
        // ���Լ��벢������ӽ��
        return hobbies.insert(like);
    }

    // ɾ������
    bool remove_hobby(const Hobby &like) {
        return hobbies.remove(like);
    }

#pragma region Ovreload
    // ����<�����
    friend bool operator<(const Person &a, const Person &b) {
        return a.id < b.id;
    }
    friend bool operator<(const Person &a, const ID &a_id) {
        return a.id < a_id;
    }
    friend bool operator<(const ID &a_id, const Person &a) {
        return a_id < a.id;
    }

    // ����>�����
    friend bool operator>(const Person &a, const Person &b) {
        return a.id > b.id;
    }
    friend bool operator>(const Person &a, const ID &a_id) {
        return a.id > a_id;
    }
    friend bool operator>(const ID &a_id, const Person &a) {
        return a_id > a.id;
    }

    // ����==�����
    friend bool operator==(const Person &a, const Person &b) {
        return a.id == b.id;
    }

    // ����==�����
    friend bool operator==(const Person &a, const ID &a_id) {
        return a.id == a_id;
    }

    // ���ݶԳ��ԣ���������һ��==
    friend bool operator==(const ID &a_id, const Person &a) {
        return a_id == a.id;
    }

    // ����!=�����
    friend bool operator!=(const Person &a, const Person &b) {
        return a.id != b.id;
    }
    // ������������
    friend std::ostream &operator<<(std::ostream &os, const Person &a) {
        os << "ID: " << a.id << ", name: " << a.name;

        return os;
    }
#pragma endregion

};
