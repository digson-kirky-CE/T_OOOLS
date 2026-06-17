#include <iostream>
#include <string>
#include <cstring>
#include <vector>

// ============ Python风格的len()函数 ============

// 1. C字符串特化（必须先声明，避免模板匹配）
size_t len(const char* s) {
    return std::strlen(s);
}

// 2. 字符串特化
size_t len(const std::string& s) {
    return s.length();
}

// 3. STL容器（vector, list, map等）
template<typename Container>
auto len(const Container& c) -> decltype(c.size()) {
    return c.size();
}

// 4. 静态数组（编译期确定大小）
template<typename T, size_t N>
constexpr size_t len(const T (&)[N]) {
    return N;
}

// ============ 主程序 ============

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "用法: len <字符串或参数列表>" << std::endl;
        std::cerr << "示例:" << std::endl;
        std::cerr << "  len hello        # 输出: 5" << std::endl;
        std::cerr << "  len \"Hello World\"  # 输出: 11" << std::endl;
        std::cerr << "  len a b c d e    # 输出: 5 (参数个数)" << std::endl;
        return 1;
    }

    // 如果只有1个参数，返回字符串长度
    if (argc == 2) {
        std::cout << len(argv[1]) << std::endl;
    } else {
        // 多个参数，返回参数个数（不包括命令本身）
        std::cout << (argc - 1) << std::endl;
    }
    
    return 0;
}
