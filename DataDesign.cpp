#include <fstream>
#include "Person.hpp"
// 存放所有人的集合，相当于服务器，暂不支持注销账号操作，注销太麻烦了
Set<Person> users;
// None
Person none(-1, "None");
// 全部的爱好
Set<Hobby> all_hobbies;
// 基本操作
#pragma region Operations
// 通过ID搜索人，返回找到的人的引用
Person &find_by_id(const ID &id) {
    // 确保id有效
    if (id >= 0) {
        return users.find_by_id(id);
    }
    else {
        return none;
    }
}

// 获取好友集
Set<ID> get_friends(const ID &id) {
    // 直接获取引用
    Person &person = users.find_by_id(id);
    // 获取粉丝和关注
    Set<ID> fans = person.get_fans();
    Set<ID> watches = person.get_watches();
    // 粉丝就是相互关注
    return fans.intersection(watches);
}

// 获取粉丝集
Set<ID> get_fans(const ID &id) {
    // 返回粉丝集
    return users.find_by_id(id).get_fans();
}

// 获取关注集
Set<ID> get_watches(const ID &id) {
    return users.find_by_id(id).get_watches();
}

// 获取共同关注集
Set<ID> get_common_watches(const ID &a_id, const ID &b_id) {
    // 分别获取a和b的关注集
    Set<ID> a_watches = users.find_by_id(a_id).get_watches();
    Set<ID> b_watches = users.find_by_id(b_id).get_watches();
    // 取其交集
    return a_watches.intersection(b_watches);
}

// 获取共同爱好
Set<Hobby> get_common_hobbies(const ID &a_id, const ID &b_id) {
    // 分别获取a和b的爱好
    Set<Hobby> a_likes = users.find_by_id(a_id).get_hobbies();
    Set<Hobby> b_likes = users.find_by_id(b_id).get_hobbies();
    // 取交集
    return a_likes.intersection(b_likes);
}

// 获取共同好友
Set<ID> get_common_friends(const ID &a_id, const ID &b_id) {
    // 分别获取a和b的好友
    Set<ID> a_friends = get_friends(a_id);
    Set<ID> b_friends = get_friends(b_id);
    // 取交集
    return a_friends.intersection(b_friends);
}

// 获取二度好友
Set<ID> get_2d_friends(const ID &my_id) {
    // 存储所有好友的id
    std::vector<ID*> friends_ids;
    Set<ID> a_friends = get_friends(my_id);
    // 获取a的所有好友的id
    a_friends.get_all_value(friends_ids);
    // 创建集合存放结果
    Set<ID> all_friends;
    for (const ID *id : friends_ids) {
        // 获取自己好友的所有好友
        all_friends += get_friends(*id);
    }
    // 将自己的好友移除
    friends_ids.clear();
    a_friends.get_all_value(friends_ids);
    for (const ID *id : friends_ids) {
        // 移除a的好友
        all_friends.remove(*id);
    }
    // 移除a自身
    all_friends.remove(my_id);
    // 得到结果，返回
    return all_friends;
}

// 添加关注
bool add_watch(const ID &my_id, const ID &other_person) {
    Person &me = find_by_id(my_id);
    Person &other = find_by_id(other_person);
    if (me == none || other == none) {
        return false;
    }
    // 关注需要改动两方面
    return me.add_watch(other_person) && other.add_funs(my_id);
}

// 取消关注
bool cancel_watch(const ID &my_id, const ID &watch_id) {
    // 需要操作两方面，我取关别人，别人还要移除粉丝集中的我
    return users.find_by_id(my_id).cancel_watch(watch_id)
        && users.find_by_id(watch_id).remove_fans(my_id);
}

// 移除粉丝
bool remove_fans(const ID &my_id, const ID &fans_id) {
    // 分为两部分，我移除粉丝，对应的人取消关注
    return users.find_by_id(my_id).remove_fans(fans_id)
        && users.find_by_id(fans_id).cancel_watch(my_id);
}

// 获取所有爱好
Set<Hobby> get_hobbies(const ID &my_id) {
    return find_by_id(my_id).get_hobbies();
}

// 添加爱好
bool add_hobby(const ID &my_id, const Hobby &hobby) {
    Person &p = find_by_id(my_id);
    if (p != none) {
        all_hobbies.insert(hobby);
        return p.add_hobby(hobby);
    }
    return false;
}

// 取消爱好
bool remove_hobby(const ID &my_id, const Hobby &hobby) {
    // 单方面取消即可
    return users.find_by_id(my_id).remove_hobby(hobby);
}

// 从文件加载
bool load_from_file(const std::string &file_name) {
    // 打开文件
    std::fstream in(file_name, std::ios_base::in);
    // 判断打开是否成功
    if (!in.is_open()) {
        // 失败返回false
        return false;
    }
    int population;
    // 先获取人的数量
    in >> population;
    // 每个人的3个集合均以-1结束
    // 依次输入id，名称，
    // 喜好集，关注集，粉丝集
    for (int i = 0; i < population; i++) {
        ID id;
        Name name;
        // 输入id，名称
        in >> id >> name;
        Person person(id, name);
        Hobby like;
        // 输入喜好集和
        while ((in >> like, like) != "-1") {
            person.add_hobby(like);
            // 加入所有集
            all_hobbies.insert(like);
        }
        //输入关注集
        while ((in >> id, id) != -1) {
            person.add_watch(id);
        }
        //输入粉丝集
        while ((in >> id, id) != -1) {
            person.add_funs(id);
        }
        // people集合插入person
        users.insert(person);
    }
    in.close();
    //std::cout << "Input OK\n";
    //std::cout <<"Size: " << people.size() << "\n";
    // 输入成功
    return true;
}

// 保存到文件
bool save_to_file(const std::string &file_name) {
    // 先写入人数，再依次写入id，名称，喜好集，关注集，粉丝集
    // 三个集合以-1结尾
    // 打开文件，模式设置为写入
    std::fstream out(file_name, std::ios_base::out);
    // 判断是否打开成功
    if (!out.is_open()) {
        return false;
    }
    // 人物信息
    ID id;
    Name name;
    std::vector<Hobby*> hobbies;
    std::vector<ID*> watches;
    std::vector<ID*> fans;
    // 使用指针开销很小
    std::vector<Person*> all_person;
    users.get_all_value(all_person);
    out << users.size() << "\n\n";
    for (const Person *p_person : all_person) {
        // 获取对象
        const Person &person = *p_person;
        // 获取所有信息
        id = person.get_id();
        name = person.get_name();
        // 集合中的信息使用vector存储
        // 临时变量释放太快，使用局部变量
        Set<Hobby> _likes = person.get_hobbies();
        _likes.get_all_value(hobbies);
        Set<ID> _watches = person.get_watches();
        _watches.get_all_value(watches);
        Set<ID> _fans = person.get_fans();
        _fans.get_all_value(fans);
        // 写入id和名称
        out << "\n" << id << "\n" << name << "\n";
        // 写入爱好，-1结尾
        for (const Hobby *like : hobbies) {
            out << *like << " ";
        }
        out << "-1\n";
        // 写入关注集，-1结尾
        for (const ID *id : watches) {
            out << *id << " ";
        }
        out << "-1\n";
        // 写入粉丝集，-1结尾
        for (const ID *id : fans) {
            out << *id << " ";
        }
        out << "-1\n";
    }
    out.close();
    return true;
}

// 输出Person集合
void print_person(const Set<ID> &person_set) {
    std::vector<ID*> person_vec;
    person_set.get_all_value(person_vec);
    for (const ID *id : person_vec) {
        std::cout << users.find_by_id(*id) << "\n";
    }
    std::cout << "\n";
}

// 输出爱好集合
void print_hobby(const Set<Hobby> &hobbies) {
    std::vector<Hobby*> hobbies_vec;
    hobbies.get_all_value(hobbies_vec);
    for (int i = 0, size = hobbies_vec.size(); i < size; ++i) {
        // 5个一行
        std::cout << *hobbies_vec[i] << ((i + 1) % 5 == 0 ? "\n" : " ");
    }
    std::cout << "\n";
}
#pragma endregion

// 平台相关
#pragma region PlatformRelated
// 清屏函数
void cls() {
    system("cls");
}

// 等待按键
void wait() {
    getchar();
    getchar();
}
#pragma endregion

int main() {
    // 输入文件名称
    std::string file_in;
    std::cout << "Please enter file name to load(no spaces, ex: data.txt): ";
    std::cin >> file_in;
    std::cout << "Loading from file...\n";
    if (load_from_file(file_in)) {
        std::cout << "Load from file OK.\n";
        wait();
    }
    else {
        std::cout << "Load from file ERROR.\n";
        exit(-1);
    }
    
    // 操作序号
    int operation = 1;
    // 需要的操作id变量
    ID my_id, a_id, b_id;
    // 爱好集合
    Set<Hobby> hobbies;
    // 人物集合
    Set<ID> person_set;
    // 单个爱好
    Hobby hobby;
    while (operation != 0) {
        cls();
        std::cout << "            Social Relationship Manager\n";
        std::cout << "\n\n";
        std::cout << "1.Get Friends                             2.Get Fans\n";
        std::cout << "3.Get Watches                             4.Get Common Watches\n";
        std::cout << "5.Get Common Hobbies                      6.Get 2D Friends\n";
        std::cout << "7.Add Watch                               8.Cancel Watch\n";
        std::cout << "9.Remove Fans                             10.Remove Hobby\n";
        std::cout << "11.Add Hobby\n";
        std::cout << "0.exit\n";
        std::cout << "\n\n";
        std::cout << "Please enter your operation:";
        std::cin >> operation;
        cls();
        std::cout << "\t\t";
        switch (operation) {
        case 0:
            std::cout << "Exiting...\n";
            break;
        case 1:
            std::cout << "Get Friends\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            person_set = get_friends(my_id);
            std::cout << "Get results: \n";
            print_person(person_set);
            wait();
            break;
        case 2:
            std::cout << "Get Fans\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            person_set = get_fans(my_id);
            std::cout << "Get results: \n";
            print_person(person_set);
            wait();
            break;
        case 3:
            std::cout << "Get Watches\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            person_set = get_watches(my_id);
            std::cout << "Get results: \n";
            print_person(person_set);
            wait();
            break;
        case 4:
            std::cout << "Get Common Watches\n\n";
            std::cout << "Please enter a's id: ";
            std::cin >> a_id;
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            person_set = get_common_watches(a_id, b_id);
            std::cout << "Get results: \n";
            print_person(person_set);
            wait();
            break;
        case 5:
            std::cout << "Get Common Hobbies\n\n";
            std::cout << "Please enter a's id: ";
            std::cin >> a_id;
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            hobbies = get_common_hobbies(a_id, b_id);
            std::cout << "Get results: \n";
            print_hobby(hobbies);
            wait();
            break;
        case 6:
            std::cout << "Get 2D Friends\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            person_set = get_2d_friends(my_id);
            std::cout << "Get results: \n";
            print_person(person_set);
            wait();
            break;
        case 7:
            std::cout << "Add Watch\n\n";
            std::cout << "Insert b to a's watches set.\n";
            std::cout << "Please enter a's id: ";
            std::cin >> my_id;
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            if (add_watch(my_id, b_id)) {
                std::cout << "Insert watches OK.\n";
            }
            else {
                std::cout << "Insert ERROR.\n";
            }
            wait();
            break;
        case 8:
            std::cout << "Cancel Watch\n\n";
            std::cout << "Remove b from a's watches set.\n";
            std::cout << "Please enter a's id: ";
            std::cin >> my_id;
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            if (cancel_watch(my_id, b_id)) {
                std::cout << "Cancel watch OK.\n";
            }
            else {
                std::cout << "Cancel watch ERROR.\n";
            }
            wait();
            break;
        case 9:
            std::cout << "Remove fans\n\n";
            std::cout << "Remove b from a's fans set.\n";
            std::cout << "Please enter a's id: ";
            std::cin >> my_id;
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            if (remove_fans(my_id, b_id)) {
                std::cout << "Remove fans OK.\n";
            }
            else {
                std::cout << "Remove fans ERROR.\n";
            }
            wait();
            break;
        case 10:
            std::cout << "Remove Hobby\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            std::cout << "Your hobbies: \n";
            print_hobby(get_hobbies(my_id));
            std::cout << "Please enter hobby to remove: ";
            std::cin >> hobby;
            if (remove_hobby(my_id, hobby)) {
                std::cout << "Remove hobbby OK.\n";
            }
            else {
                std::cout << "Remove hobby ERROR.\n";
            }
            wait();
            break;
        case 11:
            std::cout << "Add Hobby\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            std::cout << "Other hobies or your new hobby: ";
            print_hobby(all_hobbies.difference(get_hobbies(my_id)));
            std::cout << "Please enter hobby to add: ";
            std::cin >> hobby;
            if (add_hobby(my_id, hobby)) {
                std::cout << "Add hobby OK.\n";
            }
            else {
                std::cout << "Add hobby ERROR.\n";
            }
            wait();
            break;
        default:
            break;
        }

    }

    // 询问是否存盘
    cls();
    std::cout << "Do you want to save the data to disk?\n";
    std::cout << "y/n: ";
    char choice = 'y';
    std::cin >> choice;
    // 存盘判断
    if (choice == 'y' || choice == 'Y') {
        std::string file_name;
        std::cout << "Please input file name(no spaces): ";
        // 获取文件名
        std::cin >> file_name;
        // 存储成功
        if (save_to_file(file_name)) {
            std::cout << "Save to file OK.\n";
        }
        else {
            // 失败
            std::cout << "Save to file ERROR.\n";
        }
    }

    // 结束
    std::cout << "Good bye.\n";
    wait();

    // 退出程序
    return 0;
}
