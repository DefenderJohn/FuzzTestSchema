#ifndef TEST_RES_H
#define TEST_RES_H
#include <StyledPrint.h>
#include <vector>
#include <string>
#define FP_TOLERANCE 1e-6
class TestResult {
public:
    bool success = true;
    bool isLeaf = false;
    bool isParentOfLeaf = false;
    u_int32_t testIndex = 0;
    ssize_t subTestCount = 0;
    std::vector<TestResult> subTestResults;
    std::vector<std::string> testVerboseOutput;

    TestResult(){}
    
    TestResult(ssize_t subTestCount, bool isParentOfLeaf){
        this->isParentOfLeaf = isParentOfLeaf;
        this->subTestCount = subTestCount;
        if (isParentOfLeaf)
        {
            for (ssize_t i = 0; i < subTestCount; i++)
            {
                std::string initResult = getStyledText("[?]",TextColor::YELLOW, TextStyle::BOLD);
                testVerboseOutput.push_back(initResult);
            }
        }
    }

    void appendSubTestResult(TestResult subTestRes){
        subTestResults.push_back(subTestRes);
        if (subTestRes.isLeaf)
        {
            refreshOutput();
        }
    }

    void refreshOutput(){
        deleteLastLine();
        for (size_t i = 0; i < this->subTestResults.size(); i++)
        {
            if (subTestResults.at(i).success)
            {
                testVerboseOutput[i] = getStyledText("[√]", TextColor::GREEN, TextStyle::BOLD);
            }
            else
            {
                testVerboseOutput[i] = getStyledText("[X]", TextColor::GREEN, TextStyle::BOLD);
            }
        }
        testVerboseOutput[this->subTestResults.size()] = getStyledText("[|]", TextColor::BLUE, TextStyle::BOLD);
        for (size_t i = 0; i < this->testVerboseOutput.size(); i++)
        {
            std::cout << this->testVerboseOutput.at(i);
        }
        std::cout << std::endl;
    }

    template <typename T>
    void assertEQ(T lhs, T rhs){
        this->success = success ? lhs == rhs : success;
        if (lhs != rhs)
        {
            this->testVerboseOutput.push_back(getStyledText("Expect: " + std::to_string(lhs) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }
        
    }

    template <typename T>
    void assertFEQ(T lhs, T rhs, T tolerance = FP_TOLERANCE){
        this->success = success ? lhs - rhs <= tolerance : success;
        if (lhs != rhs)
        {
            this->testVerboseOutput.push_back(getStyledText("Expect: " + std::to_string(lhs) + " with fp_tolerance of " + std::to_string(tolerance) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }  
    }

    template <typename T>
    void assertNE(T lhs, T rhs){
        this->success = success ? lhs != rhs : success;
        if (lhs == rhs)
        {
            this->testVerboseOutput.push_back(getStyledText("Expect not equal to: " + std::to_string(lhs) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }
        
    }

    template <typename T>
    void assertFNE(T lhs, T rhs, T tolerance = FP_TOLERANCE){
        this->success = success ? lhs - rhs <= tolerance : success;
        if (lhs != rhs)
        {
            this->testVerboseOutput.push_back(getStyledText("Expect not equal to: " + std::to_string(lhs) + " with fp_tolerance of " + std::to_string(tolerance) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }  
    }
};

#endif