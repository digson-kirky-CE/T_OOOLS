// print.cpp
#include <iostream>
#include <string>
using namespace std;

// 转义字符处理（完全 Python 风格）
string escape_string(const string& s) {
    string result;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '\\' && i + 1 < s.length()) {
            switch (s[i + 1]) {
                case 'n':  result += '\n'; break;
                case 't':  result += '\t'; break;
                case 'r':  result += '\r'; break;
                case '\\': result += '\\'; break;
                case '\'': result += '\''; break;
                case '\"': result += '\"'; break;
                case 'a':  result += '\a'; break;
                case 'b':  result += '\b'; break;
                case 'f':  result += '\f'; break;
                case 'v':  result += '\v'; break;
                case '0':  result += '\0'; break;
                default:   result += s[i + 1]; break;
            }
            i++; // 跳过转义字符
        } else {
            result += s[i];
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        cout << escape_string(argv[i]);
        if (i < argc - 1) cout << " ";
    }
    cout << endl;
    return 0;
}
