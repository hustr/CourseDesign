// 二叉树测试文件
#include <ctime>
#include <set>
#include "AVLTree.hpp"


int main() {
    using namespace std;
    AVLTree<int> tree;
    // 测试插入
    // 随机生成测试数据
    set<int> num_set;
    cout << "Test insert function: \n";
    // 生成数据
    int nums_size = 10000;
    constexpr int MOD = 20000;
    for (int i = 0; i < nums_size; ++i) {
        // 生成数据对MOD取模
        num_set.insert(rand() % MOD);
    }

    // 全部插入
    for (const int &num : num_set) {
        // 时刻检测树的平衡状态
        if (!tree.Insert(num) || !tree.is_balance()) {
            cout << "Test insert failed\n";
            exit(-1);
        }
    }
    cout << "Test insert ok.\n";

    // 测试删除
    cout << "Test delete function: \n";
    // 测试删除set中一半数据
    int delete_cnt = num_set.size() / 2;
    int cnt = 0;
    auto it = num_set.begin();
    while (cnt < delete_cnt) {
        // 测试删除，时刻检测平衡
        if (!tree.Delete(*it) || !tree.is_balance()) {
            cout << "Test delete failed.\n";
            exit(-1);
        }
        num_set.erase(it++);
        ++cnt;
    }
    cout << "Test delete ok.\n";

    // 测试查找
    cout << "Test search function.\n";
    //随机查找数字，判断在set中和在tree中是否一致

    int search_size = nums_size * 2;
    for (int i = 0; i < search_size; ++i) {
        int num = rand() % (2 * MOD);
        // 如果两者查找结果不同测试失败
        if (num_set.find(num) != num_set.end() && !tree.Search(num) || 
        num_set.find(num) == num_set.end() && tree.Search(num)) {
            cout << "Test search failed.\n";
            exit(-1);
        }
    }
    cout << "Test search ok.\n";

    cout << "Test End.\n";
    return 0;
}

