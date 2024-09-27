#ifndef FUZZ_TEST_SCHEMA_H
#define FUZZ_TEST_SCHEMA_H
#include <TestResult.h>

class GenericTestClass {
protected:
    TestResult testResult;
    std::vector<GenericTestClass> sublevelTestClasses;

public:
    void* dataPtr;
    GenericTestClass() {}
    virtual ~GenericTestClass() {}
    virtual TestResult ProceedTest(NameType testName) = 0;
};

class TestDriverClass : public GenericTestClass {
protected:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual TestResult RunTest(std::string testName) = 0;
public:
    TestResult ProceedTest(NameType testName){
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
    TestResult ProceedTest(NameType testName) override {};
    ~TestContainerClass() override {}
};

class TestExecutorClass : public GenericTestClass {
public:
    ssize_t testIndex;
    TestExecutorClass(void* dataPtr, ssize_t testIndex){
        this->dataPtr = dataPtr;
        this->testIndex = testIndex;
    }
    TestResult ProceedTest(NameType testName) override {};
    ~TestExecutorClass() override {}
};

#endif