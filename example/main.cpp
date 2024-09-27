#include "FuzzTestSchema.h"
#include <unistd.h>

using BaseType = std::string;
using ContainerDatatype = std::vector<BaseType>;
using ContainerType = std::pair<ContainerDatatype, NameType>;
using DriverDatatype = std::vector<ContainerType>;
using DriverType = std::pair<DriverDatatype, NameType>;

class ExampleTestExecutorClass : public TestExecutorClass
{
public:
    using TestExecutorClass::TestExecutorClass;
    TestResult ProceedTest(NameType testName) override
    {
        this->testResult.isLeaf = true;
        this->testResult.testIndex = this->testIndex;
        std::string *data = DATA_PTR(std::string);
        // 只是一个演示样例，检查str的长度是否为零
        int length = data->length();
        this->testResult.assertNE("string length",length, 8);
        usleep(75000); // 假设有一个比较复杂的计算被执行
        return this->testResult;
    }
};

class ExampleTestContainerClass : public TestContainerClass
{
public:
    using TestContainerClass::TestContainerClass;
    TestResult ProceedTest(NameType testName) override
    {
        NameType subTestName = DATA_PTR(ContainerType)->second;
        TestResult result(DATA_PTR(ContainerType)->first.size(), true, testName + "." + subTestName);
        this->testResult = result;
        for (size_t i = 0; i < DATA_PTR(ContainerType)->first.size(); i++)
        {
            BaseType subData = DATA_PTR(ContainerType)->first.at(i);
            ExampleTestExecutorClass subClass(&subData, i);                            
            TestResult result = subClass.ProceedTest(testName + "." + subTestName); 
            this->testResult.appendSubTestResult(result);  
        }
        this->testResult.finishSubtestBatch();
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