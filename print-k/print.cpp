// print.cpp - 终极版
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>

using namespace std;
using namespace chrono;

// ============ 颜色定义 ============
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK   "\033[5m"
#define REVERSE "\033[7m"

// ============ 配置结构 ============
struct Config {
    string sep = " ";
    string end = "\n";
    string color = "";
    string prefix = "";
    string suffix = "";
    bool timestamp = false;
    bool timestamp_ms = false;
    bool uppercase = false;
    bool lowercase = false;
    bool reverse = false;
    bool center = false;
    bool right_align = false;
    bool line_number = false;
    bool no_newline = false;
    bool debug = false;
    bool json = false;
    bool csv = false;
    bool repeat = false;
    int repeat_count = 1;
    int indent = 0;
    int width = 0;
    string date_format = "";
    vector<string> files;
};

// ============ 工具函数 ============
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
                case 'e':  result += '\033'; break;
                default:   result += s[i + 1]; break;
            }
            i++;
        } else {
            result += s[i];
        }
    }
    return result;
}

string colorize(const string& text, const string& color) {
    if (color.empty()) return text;
    return color + text + RESET;
}

string get_timestamp(bool ms = false) {
    auto now = system_clock::now();
    auto time_t = system_clock::to_time_t(now);
    auto ms_part = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    
    stringstream ss;
    ss << put_time(localtime(&time_t), "[%Y-%m-%d %H:%M:%S");
    if (ms) ss << "." << setfill('0') << setw(3) << ms_part.count();
    ss << "] ";
    return ss.str();
}

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col > 0 ? w.ws_col : 80;
}

string center_text(const string& text, int width) {
    if (width <= 0) return text;
    int padding = max(0, width - (int)text.length());
    int left = padding / 2;
    int right = padding - left;
    return string(left, ' ') + text + string(right, ' ');
}

string align_right(const string& text, int width) {
    if (width <= 0) return text;
    int padding = max(0, width - (int)text.length());
    return string(padding, ' ') + text;
}

// ============ 主要处理函数 ============
void process_arguments(int argc, char *argv[], Config& cfg) {
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "-s" && i+1 < argc) {
            cfg.sep = escape_string(argv[++i]);
        }
        else if (arg == "-e" && i+1 < argc) {
            cfg.end = escape_string(argv[++i]);
        }
        else if (arg == "-c" && i+1 < argc) {
            string color = argv[++i];
            if (color == "red") cfg.color = RED;
            else if (color == "green") cfg.color = GREEN;
            else if (color == "yellow") cfg.color = YELLOW;
            else if (color == "blue") cfg.color = BLUE;
            else if (color == "magenta") cfg.color = MAGENTA;
            else if (color == "cyan") cfg.color = CYAN;
            else if (color == "white") cfg.color = WHITE;
            else if (color == "bold") cfg.color = BOLD;
            else if (color == "underline") cfg.color = UNDERLINE;
            else cfg.color = "";
        }
        else if (arg == "-p" && i+1 < argc) {
            cfg.prefix = escape_string(argv[++i]);
        }
        else if (arg == "-u" && i+1 < argc) {
            cfg.suffix = escape_string(argv[++i]);
        }
        else if (arg == "-i" && i+1 < argc) {
            cfg.indent = stoi(argv[++i]);
        }
        else if (arg == "-w" && i+1 < argc) {
            cfg.width = stoi(argv[++i]);
        }
        else if (arg == "-r" && i+1 < argc) {
            cfg.repeat = true;
            cfg.repeat_count = stoi(argv[++i]);
        }
        else if (arg == "-t") {
            cfg.timestamp = true;
        }
        else if (arg == "-T") {
            cfg.timestamp_ms = true;
        }
        else if (arg == "-upper") {
            cfg.uppercase = true;
        }
        else if (arg == "-lower") {
            cfg.lowercase = true;
        }
        else if (arg == "-rev") {
            cfg.reverse = true;
        }
        else if (arg == "-center") {
            cfg.center = true;
        }
        else if (arg == "-right") {
            cfg.right_align = true;
        }
        else if (arg == "-n") {
            cfg.no_newline = true;
        }
        else if (arg == "-line") {
            cfg.line_number = true;
        }
        else if (arg == "-json") {
            cfg.json = true;
        }
        else if (arg == "-csv") {
            cfg.csv = true;
        }
        else if (arg == "-debug") {
            cfg.debug = true;
        }
        else if (arg == "--help") {
            cout << R"(
=== WORLD CLASS print COMMAND ===

USAGE:
  print [OPTIONS] [TEXT...]

OPTIONS:
  -s SEP      分隔符 (默认: 空格)
  -e END      结束符 (默认: 换行)
  -c COLOR    颜色 (red, green, blue, yellow, magenta, cyan, white, bold, underline)
  -p PREFIX   前缀
  -u SUFFIX   后缀
  -i N        缩进 N 个空格
  -w N        宽度对齐
  -r N        重复 N 次
  -t          显示时间戳
  -T          显示毫秒时间戳
  -upper      转大写
  -lower      转小写
  -rev        反转字符串
  -center     居中
  -right      右对齐
  -n          不换行
  -line       显示行号
  -json       输出 JSON 格式
  -csv        输出 CSV 格式
  -debug      调试模式

EXAMPLES:
  print "Hello\nWorld"
  print -c red "Error!" -c green "Success!"
  print -s "," -e "!\n" a b c
  print -t -p "[LOG] " "Message"
  print -center -w 80 "Centered Text"
  print -r 3 "Repeat me!"
  print -upper "hello world"
  print -json "name" "value"
  
)";
            exit(0);
        }
        else if (arg[0] != '-') {
            cfg.files.push_back(arg);
        }
    }
}

// ============ JSON 输出 ============
string to_json(const vector<string>& args) {
    stringstream ss;
    ss << "{";
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0) ss << ",";
        ss << "\"arg" << i << "\":\"" << escape_string(args[i]) << "\"";
    }
    ss << "}";
    return ss.str();
}

// ============ 主函数 ============
int main(int argc, char *argv[]) {
    Config cfg;
    process_arguments(argc, argv, cfg);
    
    // 如果没有参数，只输出空行
    if (cfg.files.empty()) {
        if (!cfg.no_newline) cout << cfg.end;
        return 0;
    }
    
    string output;
    
    // 构建输出
    for (size_t i = 0; i < cfg.files.size(); i++) {
        string text = cfg.files[i];
        
        // 转义处理
        text = escape_string(text);
        
        // 大小写转换
        if (cfg.uppercase) {
            transform(text.begin(), text.end(), text.begin(), ::toupper);
        }
        if (cfg.lowercase) {
            transform(text.begin(), text.end(), text.begin(), ::tolower);
        }
        
        // 反转
        if (cfg.reverse) {
            reverse(text.begin(), text.end());
        }
        
        // 添加前缀/后缀
        if (cfg.timestamp) text = get_timestamp(false) + text;
        if (cfg.timestamp_ms) text = get_timestamp(true) + text;
        if (cfg.line_number) {
            static int line = 1;
            text = to_string(line++) + ": " + text;
        }
        
        // 颜色
        text = colorize(text, cfg.color);
        
        // 对齐
        int width = cfg.width > 0 ? cfg.width : get_terminal_width();
        if (cfg.center) text = center_text(text, width);
        if (cfg.right_align) text = align_right(text, width);
        
        // 缩进
        if (cfg.indent > 0) {
            text = string(cfg.indent, ' ') + text;
        }
        
        // 添加前缀/后缀
        if (!cfg.prefix.empty()) text = cfg.prefix + text;
        if (!cfg.suffix.empty()) text = text + cfg.suffix;
        
        output += text;
        if (i < cfg.files.size() - 1) output += cfg.sep;
    }
    
    // 重复输出
    if (cfg.repeat) {
        for (int i = 0; i < cfg.repeat_count; i++) {
            cout << output;
            if (i < cfg.repeat_count - 1) cout << cfg.sep;
        }
    } else {
        cout << output;
    }
    
    // 结束符
    if (!cfg.no_newline) cout << cfg.end;
    
    // 调试模式
    if (cfg.debug) {
        cerr << "\n[DEBUG] Args: " << argc-1 << endl;
        cerr << "[DEBUG] Sep: '" << cfg.sep << "'" << endl;
        cerr << "[DEBUG] End: '" << cfg.end << "'" << endl;
        cerr << "[DEBUG] Color: '" << cfg.color << "'" << endl;
        cerr << "[DEBUG] Output length: " << output.length() << endl;
    }
    
    return 0;
}
