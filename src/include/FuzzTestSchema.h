#ifndef FUZZ_TEST_SCHEMA_H
#define FUZZ_TEST_SCHEMA_H
#include <TestResult.h>
class TestDriverClass;
class TestContainerClass;
class TestExecutorClass;


class GenericTestClass {
protected:
    void* dataPtr;
    TestResult testResult;
    std::vector<TestDriverClass>    sublevelTestDrivers;
    std::vector<TestContainerClass> sublevelTestContainers;
    std::vector<TestExecutorClass>  sublevelTestExecutors;

public:
    GenericTestClass() {}
    virtual ~GenericTestClass() {}
    virtual TestResult ProceedTest() = 0;
};

class TestDriverClass : public GenericTestClass {
protected:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual TestResult runTest() = 0;
public:
    TestResult ProceedTest(){
        SetUp();
        this->testResult.appendSubTestResult(runTest());
        TearDown();
        return this->testResult;
    }
    ~TestDriverClass(){
        this->TearDown();
        if (this->dataPtr != nullptr)
        {
            printStyledText("!! TearDown函数未将dataPtr清理并置为nullPtr, 可能导致内存泄漏 !!", TextColor::RED, TextStyle::BOLD);
        }
    };
};

class TestContainerClass : public GenericTestClass {
public:
    bool isParentOfLeaf;
    TestContainerClass(bool isParentOfLeaf){
        this->isParentOfLeaf = isParentOfLeaf;
    }
    virtual TestResult ProceedTest() override;
};

class TestExecutorClass : public GenericTestClass {
public:
    virtual TestResult ProceedTest() override;
};

#endif