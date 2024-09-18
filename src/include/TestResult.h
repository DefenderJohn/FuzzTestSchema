#include <vector>
#include <string>
class TestResult {
public:
    bool success;
    bool isLeaf;
    u_int32_t currentTestIndex;
    u_int32_t totalSubTests;
    std::vector<u_int32_t> failedSubTests;
    std::vector<std::string> testOutput;
    void appendSubTestResult(TestResult &subTestRes){
        if (subTestRes.isLeaf)
        {
            
        }
        
    }
};