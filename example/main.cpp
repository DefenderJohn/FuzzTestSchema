#include "FuzzTestSchema.h"
#include <unistd.h>

// 使用 BaseType 来替代 std::string，使得代码更具语义化。
using BaseType = std::string;

// ContainerDatatype 定义了一个包含多个 BaseType 的容器。
// 例如，它可以是一个包含多个字符串的向量。
using ContainerDatatype = std::vector<BaseType>;

// ContainerType 定义了一个包含 ContainerDatatype 和一个名称的配对。
// 这个配对可以用来表示一组数据和与之相关的名称。
// 例如，它可以是一个包含多条记录的容器和一个描述性的测试组名。
using ContainerType = std::pair<ContainerDatatype, NameType>;

// DriverDatatype 定义了一个包含多个 ContainerType 的容器。
// 例如，它可以是一个包含多个数据组的向量，每个数据组都有自己的名称。
using DriverDatatype = std::vector<ContainerType>;

// DriverType 定义了一个包含 DriverDatatype 和一个名称的配对。
// 这个配对可以用来表示一组数据集合和与之相关的名称。
// 例如，它可以是一个包含多个数据组集合的容器和一个描述性的测试组名。
using DriverType = std::pair<DriverDatatype, NameType>;


/**
 * @brief 示例测试执行者类。
 *
 * 此类为 `TestExecutorClass` 的具体子类，用于执行单个测试。它实现了 `ProceedTest` 方法，
 * 用于执行具体的测试逻辑。在此示例中，测试逻辑检查一个字符串的长度是否不等于8。
 *
 * 注意：
 * 用户不应该直接继承 `GenericTestClass`，而应该继承此类或其他具体的子类，以利用其提供的测试框架。
 */
class ExampleTestExecutorClass : public TestExecutorClass
{
public:
    /// @brief 使用基类的构造函数。
    /// @details 此构造函数使用基类 `TestExecutorClass` 的构造函数。
    using TestExecutorClass::TestExecutorClass;

    /**
     * @brief 执行具体的测试逻辑。
     *
     * 此方法用于执行具体的测试逻辑。在此示例中，它检查字符串的长度是否不等于8。
     *
     * @param testName 测试的名称。
     * @return 返回测试结果。
     */
    TestResult ProceedTest(NameType testName) override
    {
        // 标记当前测试为叶子节点
        this->testResult.isLeaf = true;

        // 设置当前测试的索引号
        this->testResult.testIndex = this->testIndex;

        // 使用宏定义 DATA_PTR 将 this->dataPtr 转换为 std::string 指针
        std::string *data = DATA_PTR(std::string);

        // 只是一个演示样例，检查字符串的长度是否为8
        int length = data->length();

        // 断言字符串的长度不等于8
        this->testResult.assertNE("string length", length, 8);

        // 模拟一个耗时较长的操作以便观察测试是否正常执行（在实际使用时不应这么操作）
        usleep(75000); 

        // 返回测试结果
        return this->testResult;
    }
};


/**
 * @brief 示例测试容器类。
 *
 * 此类为 `TestContainerClass` 的具体子类，用于管理多个测试执行者类。
 * 它实现了 `ProceedTest` 方法，用于执行一系列子测试。
 *
 * 注意：
 * 用户不应该直接继承 `GenericTestClass`，而应该继承此类或其他具体的子类，以利用其提供的测试框架。
 */
class ExampleTestContainerClass : public TestContainerClass
{
public:
    /// @brief 使用基类的构造函数。
    /// @details 此构造函数使用基类 `TestContainerClass` 的构造函数。
    using TestContainerClass::TestContainerClass;

    /**
     * @brief 执行一系列子测试。
     *
     * 此方法用于执行一系列子测试。它首先获取当前容器的数据，并为每个子数据创建一个 `ExampleTestExecutorClass` 实例，
     * 然后执行每个子测试并将结果合并到当前测试结果中。
     *
     * @param testName 测试的名称。
     * @return 返回测试结果。
     */
    TestResult RunTest(NameType testName) override
    {
        // 获取当前容器的名称
        NameType subTestName = DATA_PTR(ContainerType)->second;

        // 创建测试结果，设置子测试的数量，并标记为叶子节点的父节点
        TestResult result(DATA_PTR(ContainerType)->first.size(), true, testName + "." + subTestName);

        // 设置当前测试的结果为新创建的测试结果
        this->testResult = result;

        // 遍历当前容器中的所有子数据
        for (size_t i = 0; i < DATA_PTR(ContainerType)->first.size(); i++)
        {
            // 获取当前子数据
            BaseType subData = DATA_PTR(ContainerType)->first.at(i);

            // 创建一个新的 `ExampleTestExecutorClass` 实例
            ExampleTestExecutorClass subClass(&subData, i);

            // 执行子测试
            TestResult subTestResult = subClass.ProceedTest(testName + "." + subTestName);

            // 将子测试的结果附加到当前测试结果中
            this->testResult.appendSubTestResult(subTestResult);
        }

        // 返回当前测试的结果
        return this->testResult;
    }
};

class ExampleTestDriverClass : public TestDriverClass
{
protected:
    void fillData()
    {
        auto matrix = DATA_PTR(DriverType);
        DriverDatatype data;

        for (int i = 0; i < 20; ++i)
        {
            ContainerDatatype row;
            for (int j = 0; j < 50; ++j)
            {
                row.push_back("String " + std::to_string(i * 3 + j));
            }
            data.push_back(ContainerType(row, "test-" + std::to_string(i)));
        }
        matrix->first = data;
        matrix->second = "ExampleTest";
    }

    void SetUp() override
    {
        this->dataPtr = new DriverType();
        this->fillData();
    }

    TestResult RunTest(NameType testName) override
    {
        for (size_t i = 0; i < DATA_PTR(DriverType)->first.size(); i++)
        {
            ContainerType subData = DATA_PTR(DriverType)->first.at(i);
            NameType subTestName = DATA_PTR(DriverType)->second;
            ExampleTestContainerClass subClass(true, &subData);                            
            TestResult result = subClass.ProceedTest(testName + "." + subTestName); 
            this->testResult.appendSubTestResult(result);   
        }
        return this->testResult;
    }

    void TearDown() override
    {
        delete DATA_PTR(DriverType);
    }
};

int main(int argc, char **argv)
{
    ExampleTestDriverClass rootClass;
    rootClass.ProceedTest("ShowCase");
}