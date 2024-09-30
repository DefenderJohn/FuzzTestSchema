#ifndef TEST_RES_H
#define TEST_RES_H
#include <StyledPrint.h>
#include <vector>
#include <string>
#include <chrono>
#include <limits>
#include <Macros.h>

/**
 * @brief 测试结果类。
 *
 * TestResult 类用于封装单个测试的结果信息。它可以用来跟踪测试的状态，
 * 包括测试是否成功、测试名称、子测试结果等。此外，它还提供了输出测试结果的功能。
 */
class TestResult {
private:
    /// @brief 标记是否首次输出测试信息。
    /// @details 在第一次输出测试信息之前设置为 true，用于控制输出格式。
    bool firstOutput = false;

public:
    /// @brief 测试是否成功。
    /// @details 记录当前测试是否成功的标志。
    bool success = true;

    /// @brief 标记当前测试是否为叶子节点。
    /// @details 叶子节点表示没有子测试的测试。
    bool isLeaf = false;

    /// @brief 标记当前测试是否为叶子节点的父节点。
    /// @details 如果为 true，则当前测试有一个或多个子测试。
    bool isParentOfLeaf = false;

    /// @brief 当前测试的索引号。
    /// @details 用于标识测试序列中的位置。
    u_int32_t testIndex = 0;

    /// @brief 子测试的数量。
    /// @details 记录当前测试包含的子测试数量。
    ssize_t subTestCount = 0;

    /// @brief 失败的子测试数量。
    /// @details 记录当前测试中失败的子测试数量。
    ssize_t failedCount = 0;

    /// @brief 测试的名称。
    /// @details 描述当前测试的名称。
    std::string testName;

    /// @brief 子测试结果的集合。
    /// @details 包含当前测试的所有子测试结果。
    std::vector<TestResult> subTestResults;

    /// @brief 详细的测试输出信息。
    /// @details 包含测试过程中的详细输出信息，如进度符号等。
    std::vector<std::string> testVerboseOutput;

    /// @brief 错误信息集合。
    /// @details 收集测试过程中遇到的所有错误信息。
    std::vector<std::string> errorInfo;

    uint64_t runTime;

    /**
     * 喂给自动构造器的空构造函数
     * 除非明确在调用后手动初始化变量，否则不应当手动调用这个方法
     */
    TestResult(){}
    
    /**
    * @brief 初始化 TestResult 对象。
    *
    * 此构造函数用于创建一个新的 TestResult 实例，并根据是否为叶子节点的父节点来初始化相关属性。
    *
    * @param subTestCount 子测试的数量。
    * @param isParentOfLeaf 是否为叶子节点的父节点。
    * @param testName 测试的名称。
    */
    TestResult(ssize_t subTestCount, bool isParentOfLeaf, std::string testName){
        this->isParentOfLeaf = isParentOfLeaf;
        this->subTestCount = subTestCount;
        this->testName = testName;
        if (isParentOfLeaf)
        {
            for (ssize_t i = 0; i < subTestCount; i++)
            {
                std::string initResult = getStyledText("[?]",TextColor::YELLOW, TextStyle::BOLD);
                testVerboseOutput.push_back(initResult);
            }
        }
    }

    /**
     * @brief 结束一批子测试，并汇总结果。
     *
     * 此方法用于处理一批子测试结束后的工作，包括汇总各个子测试的结果、删除之前的输出行、
     * 并最终输出整个测试批次的结果。如果所有子测试都成功，则输出成功信息；如果有失败的子测试，
     * 则输出失败信息及具体的错误信息。
     *
     * 注意：这是内部使用的方法，不建议外部调用。
     */
    void finishSubtestBatch(){
        deleteLastLine();
        deleteLastLine();
        for (auto subtestRes : this->subTestResults)
        {
            this->success = this->success ? subtestRes.success : this->success;
            for (auto subErrorInfo : subtestRes.errorInfo)
            {
                this->errorInfo.push_back(subErrorInfo);
            }
            this->failedCount += subtestRes.failedCount;
        }
        
        if (this->success)
        {
            uint64_t longestRunTime = 0;
            uint64_t shortestRunTime = std::numeric_limits<uint64_t>::max();
            uint64_t averageRunTime = 0;
            for (size_t i = 0; i < this->subTestResults.size(); i++)
            {
                auto subtestTime = this->subTestResults.at(i).runTime;
                longestRunTime = subtestTime > longestRunTime ? subtestTime : longestRunTime;
                shortestRunTime = subtestTime < shortestRunTime ? subtestTime : shortestRunTime;
                averageRunTime = i == 0 ? subtestTime : (int64_t)averageRunTime + ((int64_t)subtestTime - (int64_t)averageRunTime) / (int64_t)(i + 1);
            }

            std::string longestRunTimeStr = formatTime(longestRunTime);
            std::string shortestRunTimeStr = formatTime(shortestRunTime);
            std::string averageRunTimeStr = formatTime(averageRunTime);
            
            printStyledText("Test: " + testName + "  SUCCEED(" + std::to_string(subTestCount) + " passed)", TextColor::GREEN, TextStyle::NORMAL, false);
            printStyledText(" : Average running time: " + averageRunTimeStr + ", Longest running time: " + longestRunTimeStr + ", Shortest running time: " + shortestRunTimeStr + ".", TextColor::CYAN, TextStyle::NORMAL, true);
        }else{
            printStyledText("Test: " + testName + "  FAILED(" + std::to_string(subTestCount) + " passed, " + std::to_string(failedCount) + " failed)", TextColor::RED, TextStyle::BOLD, true);
            for (auto error : errorInfo)
            {
                printStyledText(error, TextColor::RED, TextStyle::ITALIC, true);
            }
            
        } 
    }

    /**
    * @brief 向测试结果集合中追加子测试结果。
    *
    * 此方法用于将一个新的子测试结果添加到测试结果集合中，并在子测试是叶子节点时刷新输出状态。
    *
    * @param subTestRes 子测试结果对象。
    *
    * 注意：这是内部使用的方法，外部调用可能会破坏测试状态。
    */
    void appendSubTestResult(TestResult subTestRes){
        subTestResults.push_back(subTestRes);
        if (subTestRes.isLeaf)
        {
            refreshOutput();
        }
    }

    /**
    * @brief 更新测试输出状态。
    *
    * 此方法用于更新控制台中的测试状态显示，包括清除旧行、打印测试名称和子测试结果符号。
    * 成功的子测试会显示绿色的 "[√]"，失败的子测试会显示红色的 "[X]"，正在进行的子测试会显示蓝色的 "[|]"。
    *
    * 注意：这是内部使用的方法，外部调用可能会破坏测试状态。
    */
    void refreshOutput(){
        if(firstOutput)
        {
            deleteLastLine();
        }else{
            printStyledText("Test: " + testName + "  RUNNING", TextColor::BLUE, TextStyle::NORMAL, true);
        }
        this->firstOutput = true;
        for (size_t i = 0; i < this->subTestResults.size(); i++)
        {
            if (subTestResults.at(i).success)
            {
                testVerboseOutput[i] = getStyledText("[√]", TextColor::GREEN, TextStyle::BOLD);
            }
            else
            {
                testVerboseOutput[i] = getStyledText("[X]", TextColor::RED, TextStyle::BOLD);
            }
        }
        if (this->subTestResults.size() < this->testVerboseOutput.size())
        {
            testVerboseOutput[this->subTestResults.size()] = getStyledText("[|]", TextColor::BLUE, TextStyle::BOLD);
        }
        for (size_t i = 0; i < this->testVerboseOutput.size(); i++)
        {
            std::cout << this->testVerboseOutput.at(i);
        }
        std::cout << std::endl;
    }

    /**
    * @brief 断言两个给定值是否相等，并记录错误信息。
    *
    * 此函数用于比较两个同类型的值（lhs 和 rhs），如果它们不相等，则记录一条错误信息，
    * 并更新测试的成功状态。此函数不会中断测试执行。
    *
    * @param variableName 变量名字符串，用于在错误信息中标识比较对象。
    * @param lhs 左手边表达式的值。
    * @param rhs 右手边表达式的期望值。
    *
    */
    template <typename T>
    void assertEQ(std::string variableName, T lhs, T rhs){
        this->success = success ? lhs == rhs : success;
        if (lhs != rhs)
        {
            this->errorInfo.push_back(getStyledText("Test " + std::to_string(this->testIndex) +": Expect " + variableName + " = " + std::to_string(lhs) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }   
    }

    /**
    * @brief 断言两个浮点数是否在指定容差范围内近似相等。
    *
    * 此函数用于验证两个浮点数值（lhs 和 rhs）是否相差不超过一个给定的容差值。
    * 容差值由FP_TOLERANCE宏定义，默认为1e-6。
    * 如果两个值的差异超出容差范围，则记录一条错误信息。
    *
    * @param variableName 变量名字符串，用于在错误信息中标识比较对象。
    * @param lhs 左手边表达式的值。
    * @param rhs 右手边表达式的期望值。
    * @param tolerance 比较时使用的容差，默认值为 FP_TOLERANCE。
    *
    * 示例：
    * 假设 `tolerance` 的值为 0.001，则调用 `assertFEQ("value", 0.1, 0.10001)` 将被视为通过。
    */
    template <typename T>
    void assertFEQ(std::string variableName, T lhs, T rhs, T tolerance = FP_TOLERANCE){
        this->success = success ? ABS(lhs, rhs) <= tolerance : success;
        this->failedCount = !success ? failedCount + 1 : failedCount;
        if (lhs != rhs)
        {
            this->errorInfo.push_back(getStyledText("Test " + std::to_string(this->testIndex) +": Expect " + variableName + " = " + std::to_string(lhs) + " with fp_tolerance of " + std::to_string(tolerance) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }  
    }

    /**
    * @brief 断言两个给定值是否不相等。
    *
    * 此函数用于验证两个同类型值（lhs 和 rhs）是否不相等。如果不满足条件（即两个值相等），
    * 则记录一条错误信息，并更新测试的失败计数。
    *
    * @param variableName 变量名字符串，用于在错误信息中标识比较对象。
    * @param lhs 左手边表达式的值。
    * @param rhs 右手边表达式的期望值。
    *
    * 示例：
    * 调用 `assertNE("value", 1, 2)` 将被视为通过，因为两个值不相等。
    * 调用 `assertNE("value", 1, 1)` 将被视为失败。
    */
    template <typename T>
    void assertNE(std::string variableName, T lhs, T rhs){
        this->success = success ? lhs != rhs : success;
        this->failedCount = !success ? failedCount + 1 : failedCount;
        if (lhs == rhs)
        {
            this->errorInfo.push_back(getStyledText("Test " + std::to_string(this->testIndex) +": Expect " + variableName + " ≠ " + std::to_string(lhs) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }
        
    }

    /**
    * @brief 断言两个浮点数是否不在指定容差范围内近似相等。
    *
    * 此函数用于验证两个浮点数值（lhs 和 rhs）是否相差超过一个给定的容差值。
    * 容差值由FP_TOLERANCE宏定义，默认为1e-6。
    * 如果两个值的差异在容差范围内，则记录一条错误信息。
    *
    * @param variableName 变量名字符串，用于在错误信息中标识比较对象。
    * @param lhs 左手边表达式的值。
    * @param rhs 右手边表达式的期望值。
    * @param tolerance 比较时使用的容差，默认值为 FP_TOLERANCE。
    *
    * 示例：
    * 假设 `tolerance` 的默认值为 0.001，则调用 `assertFNE("value", 0.1, 0.10001)` 将被视为失败，
    * 因为两个值在容差范围内被认为是相等的。
    */
    template <typename T>
    void assertFNE(std::string variableName, T lhs, T rhs, T tolerance = FP_TOLERANCE){
        this->success = success ? ABS(lhs, rhs) <= tolerance : success;
        this->failedCount = !success ? failedCount + 1 : failedCount;
        if (lhs != rhs)
        {
            this->errorInfo.push_back(getStyledText("Test " + std::to_string(this->testIndex) +": Expect " + variableName + " ≠ " + std::to_string(lhs) + " with fp_tolerance of " + std::to_string(tolerance) + ", but get: " + std::to_string(rhs) + ".", TextColor::RED, TextStyle::NORMAL));
        }  
    }
};

#endif