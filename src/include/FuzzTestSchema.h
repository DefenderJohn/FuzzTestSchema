#ifndef FUZZ_TEST_SCHEMA_H
#define FUZZ_TEST_SCHEMA_H
#include <TestResult.h>

/**
 * @brief 泛用测试类基类。
 *
 * 此类为泛用测试类的基类，提供了测试结果管理和子测试类管理的基础功能。
 * 用户不应该直接继承此类，而应当继承从此类派生的子类，以利用其提供的具体测试逻辑。
 *
 * 注意：
 * 直接继承此类可能会导致不正确的测试行为。请确保从具体的子类继承。
 */
class GenericTestClass {
protected:
    /// @brief 当前测试的结果。
    TestResult testResult;

    /// @brief 子测试类的集合。
    std::vector<GenericTestClass> sublevelTestClasses;

public:
    /// @brief 数据指针，用于存储测试所需的数据。
    void* dataPtr;

    /// @brief 默认构造函数。
    GenericTestClass() {}

    /// @brief 析构函数。
    virtual ~GenericTestClass() {}

    /// @brief 执行测试的纯虚函数。
    /// @param testName 测试的名称。
    /// @return 返回测试结果。
    virtual TestResult ProceedTest(NameType testName) = 0;
};

/**
 * @brief 测试驱动类。
 *
 * 此类为 `GenericTestClass` 的具体子类，实现了测试的设置 (`SetUp`) 和清理 (`TearDown`) 功能，
 * 并提供了一个执行测试的接口 (`RunTest`)。用户应当继承此类，并实现必要的虚函数以完成测试逻辑。
 *
 * 注意：
 * 用户不应该直接继承 `GenericTestClass`，而应该继承此类或其他具体的子类，以利用其提供的测试框架。
 */
class TestDriverClass : public GenericTestClass {
protected:
    /// @brief 设置测试前的状态。
    /// @details 此函数必须在派生类中实现，用于准备测试前的环境。
    virtual void SetUp() = 0;

    /// @brief 清理测试后的状态。
    /// @details 此函数必须在派生类中实现，用于清理测试后的环境。
    virtual void TearDown() = 0;

    /// @brief 运行测试。
    /// @param testName 测试的名称。
    /// @return 返回测试结果。
    /// @details 此函数必须在派生类中实现，以执行具体的测试逻辑。
    virtual TestResult RunTest(std::string testName) = 0;

public:
    /// @brief 执行测试。
    /// @param testName 测试的名称。
    /// @return 返回测试结果。
    /// @details 此函数负责调用 `SetUp`、运行 `RunTest` 并调用 `TearDown`，最后返回测试结果。
    TestResult ProceedTest(NameType testName) {
        SetUp();
        this->testResult.appendSubTestResult(RunTest(testName));
        TearDown();
        return this->testResult;
    }

    /// @brief 析构函数。
    ~TestDriverClass() override {}
};

/**
 * @brief 测试容器类。
 *
 * 此类为 `GenericTestClass` 的具体子类，用于作为测试容器，可以包含多个子测试类。
 * 用户在需要创建一个包含多个子测试的容器时，应当继承此类，并实现必要的虚函数以完成测试逻辑。
 *
 * 注意：
 * 用户不应该直接继承 `GenericTestClass`，而应该继承此类或其他具体的子类，以利用其提供的测试框架。
 */
class TestContainerClass : public GenericTestClass {
public:
    /// @brief 标记是否为叶子节点的父节点。
    /// @details 如果为 true，则表示该容器有子测试。
    bool isParentOfLeaf;

    /// @brief 构造函数。
    /// @param isParentOfLeaf 是否为叶子节点的父节点。
    /// @param dataPtr 测试所需的数据指针。
    TestContainerClass(bool isParentOfLeaf, void* dataPtr) {
        this->isParentOfLeaf = isParentOfLeaf;
        this->dataPtr = dataPtr;
    }

    /// @brief 执行测试。
    /// @param testName 测试的名称。
    /// @return 返回测试结果。
    /// @details 此函数需要在派生类中实现，以执行具体的测试逻辑。
    TestResult ProceedTest(NameType testName) override {
        START_TIMER;
        if (isParentOfLeaf)
        {
            auto result = RunTest(testName);
            auto time = FINISH_TIMER;
            result.runTime = time;
            this->testResult.finishSubtestBatch(true);
            return result;
        }
        auto result = RunTest(testName);
        auto time = FINISH_TIMER;
        result.runTime = time;
        return result;
    };

    virtual TestResult RunTest(NameType testName) = 0;

    /// @brief 析构函数。
    ~TestContainerClass() override {}
};

/**
 * @brief 测试执行者类。
 *
 * 此类为 `GenericTestClass` 的具体子类，用于执行单个测试。用户在需要执行单个测试时，
 * 应当继承此类，并实现必要的虚函数以完成测试逻辑。
 *
 * 注意：
 * 用户不应该直接继承 `GenericTestClass`，而应该继承此类或其他具体的子类，以利用其提供的测试框架。
 */
class TestExecutorClass : public GenericTestClass {
public:
    /// @brief 当前测试的索引号。
    /// @details 用于标识当前测试在测试序列中的位置。
    ssize_t testIndex;

    /// @brief 构造函数。
    /// @param dataPtr 测试所需的数据指针。
    /// @param testIndex 当前测试的索引号。
    TestExecutorClass(void* dataPtr, ssize_t testIndex) {
        this->dataPtr = dataPtr;
        this->testIndex = testIndex;
    }

    /// @brief 执行测试。
    /// @param testName 测试的名称。
    /// @return 返回测试结果。
    /// @details 此函数需要在派生类中实现，以执行具体的测试逻辑。
    TestResult ProceedTest(NameType testName) override {
        START_TIMER;
        auto result = RunTest(testName);
        auto time = FINISH_TIMER;
        result.runTime = time;
        return result;
    };

    virtual TestResult RunTest(NameType testName) = 0;

    /// @brief 析构函数。
    ~TestExecutorClass() override {}
};

#endif