#include "Operations.hpp"

int main() {
    // 输入文件名称
    std::string file_in;
    std::cout << "Please enter file name to load: ";
    std::cin >> file_in;
    std::cout << "Loading from file...\n";
    // 读取文件
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
    // 主循环
    while (operation != 0) {
        // 清屏
        cls();
        // 显示菜单
        std::cout << "            Social Relationship Manager\n";
        std::cout << "\n\n";
        std::cout << "1.Get Friends                             2.Get Fans\n";
        std::cout << "3.Get Watches                             4.Get Common Watches\n";
        std::cout << "5.Get Common Hobbies                      6.Get 2D Friends\n";
        std::cout << "7.Add Watch                               8.Cancel Watch\n";
        std::cout << "9.Remove Fans                             10.Remove Hobby\n";
        std::cout << "11.Add Hobby                              12.Sign Up Person\n";
        std::cout << "13.Unsubscribe Person                     14.Get Hobbies\n";
        std::cout << "0.exit\n";
        std::cout << "\n\n";
        std::cout << "Please enter your operation:";
        // 获取操作
        std::cin >> operation;
        cls();
        std::cout << "\t\t";
        // 根据选择进行操作
        switch (operation) {
        case 0:
            std::cout << "Exiting...\n";
            break;
        case 1:
            // 获取朋友
            std::cout << "Get Friends\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            // 直接调用函数
            person_set = get_friends(my_id);
            std::cout << "Get " << get_name(my_id) << "'s friends: \n";
            // 打印名单
            print_person(person_set);
            wait();
            break;
        case 2:
            // 获取粉丝
            std::cout << "Get Fans\n\n";
            std::cout << "Please enter id: ";
            // 获取要获取的人的id
            std::cin >> my_id;
            // 调用函数
            person_set = get_fans(my_id);
            std::cout << "Get " << get_name(my_id) << "'s fans: \n";
            // 输出
            print_person(person_set);
            wait();
            break;
        case 3:
            // 获取某人所有的关注的人
            std::cout << "Get Watches\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            // 调用函数
            person_set = get_watches(my_id);
            std::cout << "Get  " << get_name(my_id) << "'s watches: \n";
            // 输出
            print_person(person_set);
            wait();
            break;
        case 4:
            // 获取共同爱好
            std::cout << "Get Common Watches\n\n";
            std::cout << "Please enter a's id: ";
            std::cin >> a_id;
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            // 使用函数
            person_set = get_common_watches(a_id, b_id);
            std::cout << "Get common friends between " <<
                get_name(a_id) << " and " << get_name(b_id) << ": \n";
            // 输出
            print_person(person_set);
            wait();
            break;
        case 5:
            // 获取共同爱好
            std::cout << "Get Common Hobbies\n\n";
            std::cout << "Please enter a's id: ";
            std::cin >> a_id;
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            hobbies = get_common_hobbies(a_id, b_id);
            std::cout << "Get common hobbies between " <<
                get_name(a_id) << " and " << get_name(b_id) << ": \n";
            // 输出获得的爱好集合
            print_hobbies(hobbies);
            wait();
            break;
        case 6:
            // 获取二度好友
            std::cout << "Get 2D Friends\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            // 获取
            person_set = get_2d_friends(my_id);
            std::cout << "Get all of " << get_name(my_id) << "'s 2D friends: \n";
            // 打印名单
            print_person(person_set);
            wait();
            break;
        case 7:
            // 添加关注的人
            std::cout << "Add Watch\n\n";
            std::cout << "Insert b to a's watches set.\n";
            std::cout << "Please enter a's id: ";
            // 输入此人的id
            std::cin >> my_id;
            // 输出一下已经关注的人的集合
            std::cout << "These are " << get_name(my_id) << "'s all watches: \n";
            print_person(get_watches(my_id));
            std::cout << "Please enter b's id: ";
            // 获取要去关注的人的id
            std::cin >> b_id;
            // 添加成功或者失败的操作
            if (add_watch(my_id, b_id)) {
                std::cout << "Insert " << get_name(b_id) << " into " <<
                    get_name(my_id) << "'s watches OK.\n";
            }
            else {
                std::cout << "Insert " << get_name(b_id) << " into " <<
                    get_name(my_id) << "'s watches  ERROR.\n";
            }
            wait();
            break;
        case 8:
            // 取消关注
            std::cout << "Cancel Watch\n\n";
            std::cout << "Remove b from a's watches set.\n";
            std::cout << "Please enter a's id: ";
            std::cin >> my_id;
            // 输出一下已经关注的人
            std::cout << "These are watches of " << get_name(my_id) << ": \n";
            print_person(get_watches(my_id));
            std::cout << "Please enter b's id: ";
            // 输如要取消关注的人
            std::cin >> b_id;
            if (cancel_watch(my_id, b_id)) {
                std::cout << "Remove " << get_name(b_id) << " from " <<
                    get_name(my_id) << "'s watches OK.\n";
            }
            else {
                std::cout << "Remove " << get_name(b_id) << " from " <<
                    get_name(my_id) << "'s watches ERROR.\n";
            }
            wait();
            break;
        case 9:
            // 移除粉丝
            std::cout << "Remove fans\n\n";
            std::cout << "Remove b from a's fans set.\n";
            std::cout << "Please enter a's id: ";
            std::cin >> my_id;
            std::cout << "These are fans of " << get_name(my_id) << ": \n";
            // 输出已有粉丝
            print_person(get_fans(my_id));
            std::cout << "Please enter b's id: ";
            std::cin >> b_id;
            // 输出结果
            if (remove_fans(my_id, b_id)) {
                std::cout << "Remove " << get_name(b_id) << " from "
                    << get_name(my_id) << " OK\n";
            }
            else {
                std::cout << "Remove " << get_name(b_id) << " from "
                    << get_name(my_id) << " ERROR.\n";
            }
            wait();
            break;
        case 10:
            // 移除爱好
            std::cout << "Remove Hobby\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            std::cout << "These are hobbies of " << get_name(my_id) << ": \n";
            // 输出已有爱好
            print_hobbies(get_hobbies(my_id));
            std::cout << "Please enter hobby to remove: ";
            std::cin >> hobby;
            // 尝试移除爱好
            if (remove_hobby(my_id, hobby)) {
                std::cout << "Remove " << hobby << " from " << get_name(my_id) <<
                    "'s hobbies set OK.\n";
            }
            else {
                std::cout << "Remove " << hobby << " from " << get_name(my_id) <<
                    "'s hobbies set ERROR.\n";
            }
            wait();
            break;
        case 11:
            // 添加爱好
            std::cout << "Add Hobby\n\n";
            std::cout << "Please enter id: ";
            std::cin >> my_id;
            std::cout << "Other new hobies: \n";
            // 输出其他的爱好
            print_hobbies(all_hobbies.difference(get_hobbies(my_id)));
            std::cout << "Please enter hobby to add: ";
            std::cin >> hobby;
            // 尝试加入
            if (add_hobby(my_id, hobby)) {
                std::cout << "Add " << hobby << " into " << get_name(my_id) <<
                    "'s hobbies set OK.\n";
            }
            else {
                std::cout << "Add " << hobby << " into " << get_name(my_id) <<
                    "'s hobbies set ERROR.\n";
            }
            wait();
            break;
        case 12:
            // 注册新账户
            std::cout << "Sign Up Person\n\n";
            if (sign_up()) {
                std::cout << "Sign up OK\n";
            }
            else {
                std::cout << "Sign up ERROR.\n";
            }
            wait();
            break;
        case 13:
            // 注销账户
            std::cout << "Unsubscribe Person\n\n";
            std::cout << "Please enter id: ";
            // 输入要注销的id
            std::cin >> my_id;
            // 尝试注销
            if (ubsubscribe_person(my_id)) {
                std::cout << "Unsubscribe person OK.\n";
            }
            else {
                std::cout << "Unsubscribe person ERROR.\n";
            }
            wait();
            break;
        case 14:
            // 获取所有爱好
            std::cout << "Get Hobbies\n";
            std::cout << "Please enter id: ";
            // 输入id
            std::cin >> my_id;
            std::cout << "Get " << get_name(my_id) << "'s hobbies: \n";
            // 输出爱好
            print_hobbies(get_hobbies(my_id));
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
