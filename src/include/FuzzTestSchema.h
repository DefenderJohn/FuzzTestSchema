#ifndef FUZZ_TEST_SCHEMA_H
#define FUZZ_TEST_SCHEMA_H
#include <TestResult.h>
#include <typeinfo>
class TestDriverClass;
class TestContainerClass;
class TestExecutorClass;


class GenericTestClass {
protected:
    TestResult testResult;
    std::vector<GenericTestClass> sublevelTestClasses;

public:
    void* dataPtr;
    GenericTestClass() {}
    virtual ~GenericTestClass() {}
    virtual TestResult ProceedTest() = 0;
    void pushTest(GenericTestClass testClass){
        this->sublevelTestClasses.push_back(testClass);
    }
};

class TestDriverClass : public GenericTestClass {
protected:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual TestResult RunTest() = 0;
public:
    TestResult ProceedTest(){
        SetUp();
        this->testResult.appendSubTestResult(RunTest());
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
    TestContainerClass(bool isParentOfLeaf, void* dataPtr){
        this->isParentOfLeaf = isParentOfLeaf;
        this->dataPtr = dataPtr;
    }
    virtual TestResult ProceedTest() override;
};

class TestExecutorClass : public GenericTestClass {
public:
    TestExecutorClass(void* dataPtr){
        this->dataPtr = dataPtr;
    }
    virtual TestResult ProceedTest() override;
};

#endif