#include <iostream>
#include <string>

// 文本颜色
enum class TextColor {
    BLACK = 30,        // 黑色文本
    RED = 31,          // 红色文本
    GREEN = 32,        // 绿色文本
    YELLOW = 33,       // 黄色文本
    BLUE = 34,         // 蓝色文本
    MAGENTA = 35,      // 品红文本
    CYAN = 36,         // 青色文本
    WHITE = 37,        // 白色文本
    DEFAULT = 39       // 默认颜色
};

// 文本样式
enum class TextStyle {
    NORMAL = 0,        // 正常样式
    BOLD = 1,          // 加粗样式
    DIM = 2,           // 低亮度
    ITALIC = 3,        // 斜体
    UNDERLINE = 4,     // 下划线
    REVERSE = 7,       // 反显
    HIDDEN = 8         // 隐藏
};


void printStyledText(const std::string& text, TextColor color = TextColor::DEFAULT, TextStyle style = TextStyle::NORMAL) {
    std::cout << "\033[" << static_cast<int>(style) << ';' << static_cast<int>(color) << 'm' << text << "\033[0m" << std::endl;
}

void deleteLastLine() {
    std::cout << "\033[A" << "\033[2K";
}
