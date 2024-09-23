#include <vector>
#include <string>
class TestResult {
public:
    bool success;
    bool isLeaf;
    bool isParentOfLeaf;
    u_int32_t testIndex;
    std::vector<TestResult> subTestResults;
    std::vector<std::string> testVerboseOutput;
    
    TestResult(ssize_t subTestCount, bool isParentOfLeaf){
        this->isParentOfLeaf = isParentOfLeaf;
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
    }

    void refrashOutput(ssize_t index, bool succeed){
        
    }
};