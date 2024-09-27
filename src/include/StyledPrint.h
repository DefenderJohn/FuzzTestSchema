#ifndef STYLE_PRINT_H
#define STYLE_PRINT_H
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


/**
 * @brief 打印带有颜色和样式的文本。
 *
 * 此函数用于在控制台上打印带有特定颜色和样式的文本。支持自定义文本颜色和样式，并可选择是否在文本后换行。
 *
 * @param text 要打印的文本内容。
 * @param color 文本的颜色，默认为默认颜色（无颜色）。
 * @param style 文本的样式，默认为普通样式。
 * @param newLine 是否在文本后自动换行，默认为 false（不换行）。
 *
 * 示例：
 * 调用 `printStyledText("Hello World", TextColor::GREEN, TextStyle::BOLD, true)` 将以绿色高亮加粗的形式打印 "Hello World" 并换行。
 */
void printStyledText(const std::string& text, TextColor color = TextColor::DEFAULT, TextStyle style = TextStyle::NORMAL, bool newLine = false) {
    std::cout << "\033[" << static_cast<int>(style) << ';' << static_cast<int>(color) << 'm' << text << "\033[0m" << (newLine ? "\n" : "");
}

/**
 * @brief 删除控制台上的最后一行文本。
 *
 * 此函数用于在控制台上删除上一行的内容。通过移动光标到上一行的开头，
 * 并清除整行内容来实现。
 *
 * 示例：
 * 假设控制台上有一行文本 "Previous line"，调用 `deleteLastLine()` 之后，
 * 控制台上的这一行将被清空。
 */
void deleteLastLine() {
    std::cout << "\033[A" << "\033[2K";
}

/**
 * @brief 获取带有颜色和样式的文本字符串。
 *
 * 此函数用于生成带有特定颜色和样式的文本字符串。返回的字符串可以直接打印到支持 ANSI 转义码的终端中，
 * 以显示带有指定样式和颜色的文本。
 *
 * @param text 要格式化的文本内容。
 * @param color 文本的颜色，默认为默认颜色（无颜色）。
 * @param style 文本的样式，默认为普通样式。
 *
 * 返回值：
 * 返回一个格式化后的字符串，可以直接打印到终端。
 *
 * 示例：
 * 调用 `getStyledText("Hello World", TextColor::GREEN, TextStyle::BOLD)` 将返回一个带有绿色高亮加粗格式的字符串。
 */
std::string getStyledText(const std::string& text, TextColor color = TextColor::DEFAULT, TextStyle style = TextStyle::NORMAL) {
    return "\033[" + std::to_string(static_cast<int>(style)) + ';' + std::to_string(static_cast<int>(color)) + 'm' + text + "\033[0m";
}

/**
 * @brief 在原有字符串末尾追加带有颜色和样式的文本。
 *
 * 此函数用于在原有的字符串末尾追加一段带有特定颜色和样式的文本。追加的文本将以 ANSI 转义码格式化，
 * 以便在支持此类转义码的终端中显示带有指定样式和颜色的文本。
 *
 * @param original 原始字符串，将在其末尾追加格式化后的文本。
 * @param text 要追加的文本内容。
 * @param color 文本的颜色，默认为默认颜色（无颜色）。
 * @param style 文本的样式，默认为普通样式。
 *
 * 示例：
 * 假设 `std::string original = "Original text";`，调用 `appendStyledText(original, "Bold Text", TextColor::GREEN, TextStyle::BOLD);`
 * 将使 `original` 变为 `"Original text\033[1;32mBold Text\033[0m"`，其中 "Bold Text" 部分将以绿色高亮加粗形式显示。
 */
void appendStyledText(std::string& original, const std::string& text, TextColor color = TextColor::DEFAULT, TextStyle style = TextStyle::NORMAL) {
    original += getStyledText(text, color, style);
}
#endif