#include "FuzzTestSchema.h"

int main(int argc, char **argv) {
    printStyledText("Test: BinOp.AddOp.FuzzTests.LargeFuzz    PASSED(25 succeed)",TextColor::GREEN, TextStyle::NORMAL, true);
    printStyledText("Test: BinOp.AddOp.FuzzTests.LargeFuzz    FAILED(17 succeed 2 failed)",TextColor::RED, TextStyle::NORMAL, true);
    printStyledText("Test: BinOp.AddOp.FuzzTests.HugeFuzz    RUNNING...",TextColor::BLUE, TextStyle::NORMAL, true);
    printStyledText("[√][√][√][√][√][√][√][√][√][√]", TextColor::GREEN, TextStyle::BOLD);
    printStyledText("[X]", TextColor::RED, TextStyle::BOLD);
    printStyledText("[√][√][√][√][√][√]", TextColor::GREEN, TextStyle::BOLD);
    printStyledText("[|]", TextColor::BLUE, TextStyle::BOLD);
    printStyledText("[?][?][?][?][?][?][?][?][?]", TextColor::YELLOW, TextStyle::BOLD, true);
}