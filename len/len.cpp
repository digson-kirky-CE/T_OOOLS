#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <sstream>

// 通用len函数
template<typename T>
size_t len(const T& container) {
    return container.size();
}

// C字符串特化
size_t len(const char* s) {
    return strlen(s);
}

// 解析字符串为列表（支持逗号分隔）
std::vector<std::string> parse_list(const std::string& s) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // 去除首尾空格
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        if (!item.empty()) result.push_back(item);
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "用法: len <参数>" << std::endl;
        std::cerr << "  len hello           # 5 (字符串长度)" << std::endl;
        std::cerr << "  len [1,2,3,4,5]    # 5 (列表长度)" << std::endl;
        std::cerr << "  len a b c          # 3 (多个参数)" << std::endl;
        return 1;
    }

    std::string arg = argv[1];
    
    // 检测是否为列表格式 [1,2,3]
    if (arg.front() == '[' && arg.back() == ']') {
        std::string content = arg.substr(1, arg.length() - 2);
        auto list = parse_list(content);
        std::cout << len(list) << std::endl;
    } 
    // 如果是多个参数
    else if (argc > 2) {
        std::cout << (argc - 1) << std::endl;
    } 
    // 单个参数作为字符串
    else {
        std::cout << len(argv[1]) << std::endl;
    }
    
    return 0;
}
