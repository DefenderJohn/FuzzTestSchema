#include "FuzzTestSchema.h"

class ExampleTestExecutorClass : public TestExecutorClass{
public:
    using TestExecutorClass::TestExecutorClass;
    TestResult ProceedTest() override {
        this->testResult.isLeaf = true;
        std::string* data = static_cast<std::string*>(this->dataPtr);
        // 只是一个演示样例，检查str的长度是否为零
        int length = data->length();
        this->testResult.assertNE(length, 100);
        return this->testResult;
    }
};

class ExampleTestContainerClass : public TestContainerClass{
public:
    using TestContainerClass::TestContainerClass;
    TestResult ProceedTest() override {
        this->testResult.isParentOfLeaf = true;
        for (size_t i = 0; i < (static_cast<std::vector<std::string>*>(this->dataPtr))->size(); i++)
        {
            std::string subData = (static_cast<std::vector<std::string>*>(this->dataPtr))->at(i);
            ExampleTestExecutorClass subClass(&subData);
            TestResult result = subClass.ProceedTest();
            this->testResult.appendSubTestResult(result);
        }
        return this->testResult;
    }
};

class ExampleTestDriverClass : public TestDriverClass{
protected:
    void fillData() {
        auto matrix = static_cast<std::vector<std::vector<std::string>>*>(this->dataPtr);

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

    TestResult RunTest() override {
        for (size_t i = 0; i < (static_cast<std::vector<std::vector<std::string>>*>(this->dataPtr))->size(); i++)
        {
            std::vector<std::string> subData = (static_cast<std::vector<std::vector<std::string>>*>(this->dataPtr))->at(i);
            ExampleTestContainerClass subClass(true, &subData);
            TestResult result = subClass.ProceedTest();
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
    rootClass.ProceedTest();
}