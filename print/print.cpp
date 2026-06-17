#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
    // 如果没有参数，只输出换行
    if (argc == 1) {
        cout << endl;
        return 0;
    }
    
    // 输出所有参数，用空格分隔
    for (int i = 1; i < argc; i++) {
        cout << argv[i];
        if (i < argc - 1) cout << " ";
    }
    cout << endl;
    
    return 0;
}
