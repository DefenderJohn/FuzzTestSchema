#ifndef FUZZ_TEST_SCHEMA_H
#define FUZZ_TEST_SCHEMA_H
#include <TestResult.h>
#include <Macros.h>

class GenericTestClass {
protected:
    TestResult testResult;
    std::vector<GenericTestClass> sublevelTestClasses;

public:
    void* dataPtr;
    GenericTestClass() {}
    virtual ~GenericTestClass() {}
    virtual TestResult ProceedTest(std::string testName) = 0;
};

class TestDriverClass : public GenericTestClass {
protected:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual TestResult RunTest(std::string testName) = 0;
public:
    TestResult ProceedTest(std::string testName){
        SetUp();
        this->testResult.appendSubTestResult(RunTest(testName));
        TearDown();
        return this->testResult;
    }
    ~TestDriverClass() override {}
};

class TestContainerClass : public GenericTestClass {
public:
    bool isParentOfLeaf;
    TestContainerClass(bool isParentOfLeaf, void* dataPtr){
        this->isParentOfLeaf = isParentOfLeaf;
        this->dataPtr = dataPtr;
    }
    TestResult ProceedTest(std::string testName) override {};
    ~TestContainerClass() override {}
};

class TestExecutorClass : public GenericTestClass {
public:
    TestExecutorClass(void* dataPtr){
        this->dataPtr = dataPtr;
    }
    TestResult ProceedTest(std::string testName) override {};
    ~TestExecutorClass() override {}
};

#endif