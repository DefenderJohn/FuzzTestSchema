#include "FuzzTestSchema.h"

class ExampleTestExecutorClass : public TestExecutorClass{
public:
    using TestExecutorClass::TestExecutorClass;
    TestResult ProceedTest(std::string testName) override {
        this->testResult.isLeaf = true;
        std::string* data = DATA_PTR(std::string);
        // 只是一个演示样例，检查str的长度是否为零
        int length = data->length();
        this->testResult.assertNE(length, 8);
        return this->testResult;
    }
};

class ExampleTestContainerClass : public TestContainerClass{
public:
    using TestContainerClass::TestContainerClass;
    TestResult ProceedTest(std::string testName) override {
        TestResult result(DATA_PTR(std::vector<std::string>)->size(), true);
        this->testResult = result;
        for (size_t i = 0; i < DATA_PTR(std::vector<std::string>)->size(); i++)
        {
            std::string subData = DATA_PTR(std::vector<std::string>)->at(i);
            ExampleTestExecutorClass subClass(&subData);
            TestResult result = subClass.ProceedTest(testName);
            this->testResult.appendSubTestResult(result);
        }
        return this->testResult;
    }
};

class ExampleTestDriverClass : public TestDriverClass{
protected:
    void fillData() {
        auto matrix = DATA_PTR(std::vector<std::vector<std::string>>);

        for (int i = 0; i < 5; ++i) {
            std::vector<std::string> row;
            for (int j = 0; j < 3; ++j) {
                row.push_back("String " + std::to_string(i * 3 + j));
            }
            matrix->push_back(row);
        }
    }

    void SetUp() override {
        this->dataPtr = new std::vector<std::vector<std::string>>();
        this->fillData();
    }

    TestResult RunTest(std::string testName) override {
        for (size_t i = 0; i < DATA_PTR(std::vector<std::vector<std::string>>)->size(); i++)
        {
            std::vector<std::string> subData = DATA_PTR(std::vector<std::vector<std::string>>)->at(i);
            ExampleTestContainerClass subClass(true, &subData);
            TestResult result = subClass.ProceedTest(testName);
            this->testResult.appendSubTestResult(result);
        }
        return this->testResult;
    }

    void TearDown() override {
        delete static_cast<std::vector<std::vector<std::string>>*>(this->dataPtr);
    }
};

int main(int argc, char **argv) {
    ExampleTestDriverClass rootClass;
    rootClass.ProceedTest("ExampleTest");
}