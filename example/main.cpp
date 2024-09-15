#include "FuzzTestSchema.h"

int main(int argc, char **argv) {
    printStyledText("[x]", TextColor::RED, TextStyle::BOLD);
    printStyledText("[√]", TextColor::GREEN, TextStyle::BOLD);
    printStyledText("[?]", TextColor::YELLOW, TextStyle::BOLD, true);
}