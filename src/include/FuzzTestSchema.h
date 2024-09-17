#include <StyledPrint.h>
#include <vector>
#include <TestResult.h>

class GenericTestClass {
protected:
    void* dataPtr;
    TestResult testResult;
    std::vector<TestDriverClass>    sublevelTestDrivers;
    std::vector<TestContainerClass> sublevelTestContainers;
    std::vector<TestExecutorClass>  sublevelTestExecutors;

public:
    virtual ~GenericTestClass() {}

    virtual TestResult ProceedTest() = 0;
};

class TestDriverClass : public GenericTestClass {
public:
    virtual void SetUp();
    virtual void TearDown();
    virtual TestResult ProceedTest() override;
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
    virtual TestResult ProceedTest() override;
};

class TestExecutorClass : public GenericTestClass {
public:
    virtual TestResult ProceedTest() override;
};