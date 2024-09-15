#include <StyledPrint.h>
class TestResult {
public:
    bool success;
    // TODO: 更多关于测试结果的成员和方法
};

class GenericTestClass {
protected:
    void* dataPtr;
    TestResult testResult;

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

