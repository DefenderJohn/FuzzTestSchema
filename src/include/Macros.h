#ifndef MACROS_H
#define MACROS_H

/**
 * @brief 宏定义用于安全地转换 this->dataPtr 指针。
 *
 * 此宏定义用于将 `this->dataPtr` 指针转换为指定的数据类型指针。
 * 这有助于在已知 `this->dataPtr` 指向正确的数据类型时，安全地进行类型转换。
 *
 * @param dataType 需要转换的目标数据类型。注意，这不是一个变量，而是一个类型。
 *
 * 示例：
 * 给定一个指向 void 类型的指针 `this->dataPtr`，可以通过 `DATA_PTR(std::vector<std::string>)` 获取一个指向 `std::vector<std::string>` 类型的指针。
 * 按上文举例，可以使用诸如`DATA_PTR(DriverType)->at(i)`之类的方法，也就是可以将这个表达式当做一个输入类型的指针使用（存储的值必须要符合传入类型）
 */
#define DATA_PTR(dataType) static_cast<dataType*>(this->dataPtr)

/**
 * @brief 定义浮点数比较的默认容差值。
 *
 * 此宏定义了一个常量 `FP_TOLERANCE`，用于浮点数比较时作为默认的容差值。
 * 在比较两个浮点数是否相等或近似相等时，此值作为允许的最大误差范围。
 */
#define FP_TOLERANCE 1e-6

/**
 * @brief 类型别名用于简化测试组别标签的引用。
 *
 * 此类型别名将 `std::string` 类型重新命名为 `NameType`，以便在代码中更方便地引用测试组别标签。
 */
using NameType = std::string;

/**
 * @brief 宏定义用于执行子测试并将结果附加到当前测试结果。
 *
 * 此宏用于初始化一个 `ExampleTestExecutorClass` 的实例，并通过该实例执行一个子测试。
 * 执行完成后，子测试的结果会被附加到当前测试的结果集合中。
 *
 * @param subData 用于子测试的数据。
 * @param testName 当前测试的名称。
 * @param subTestName 子测试的名称。
 *
 * 示例：
 * 假设有一个 `ExampleTestExecutorClass` 实例 `executor` 和一个测试数据 `data`，
 * 可以使用 `PROCEED_TEST(data, "mainTest", "subTest1")` 来执行名为 "mainTest.subTest1" 的子测试。
 */
#define PROCEED_TEST(subData, testName, subTestName){                       \
    ExampleTestExecutorClass subClass(&subData);                            \
    TestResult result = subClass.ProceedTest(testName + "." + subTestName); \
    this->testResult.appendSubTestResult(result);                           \
}

#define ABS(lhs, rhs) lhs > rhs ? lhs - rhs : rhs - lhs

#define START_TIMER auto start = std::chrono::high_resolution_clock::now();

#define FINISH_TIMER std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count()

// 定义转换阈值
#define MIN_THRESHOLD 300000000  // 5 minutes in microseconds
#define SEC_THRESHOLD 5000000    // 5 seconds in microseconds
#define MS_THRESHOLD 5000        // 5 milliseconds in microseconds

// 定义转换公式
#define CONVERT_MIN(time) ((float)time / 1000000.0f / 60.0f)  // Convert μs to minutes
#define CONVERT_SEC(time) ((float)time / 1000000.0f)          // Convert μs to seconds
#define CONVERT_MS(time) ((float)time / 1000.0f)              // Convert μs to milliseconds


std::string formatTime(uint64_t microseconds) {
    if (microseconds >= MIN_THRESHOLD) {
        return std::to_string(CONVERT_MIN(microseconds)) + " min";
    } else if (microseconds >= SEC_THRESHOLD) {
        return std::to_string(CONVERT_SEC(microseconds)) + " s";
    } else if (microseconds >= MS_THRESHOLD) {
        return std::to_string(CONVERT_MS(microseconds)) + " ms";
    } else {
        return std::to_string(microseconds) + " μs";
    }
}
#endif