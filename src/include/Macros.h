#ifndef MACROS_H
#define MACROS_H

#define DATA_PTR(dataType) static_cast<dataType*>(this->dataPtr)
#define FP_TOLERANCE 1e-6
using NameType = std::string;
#define PROCEED_TEST(subData, testName, subTestName){                       \
    ExampleTestExecutorClass subClass(&subData);                            \
    TestResult result = subClass.ProceedTest(testName + "." + subTestName); \
    this->testResult.appendSubTestResult(result);                           \
}

#endif