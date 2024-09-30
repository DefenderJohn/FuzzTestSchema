# FuzzTestSchema
适用于C++的多层级FuzzTest框架

# 框架说明
- TestDriverClass是负责加载和释放数据，以及调度下层测试类执行测试的对象。类比为一棵树的话，TestDriverClass便是根节点，但TestDriverClass不一定需要位于最顶端，在一些测试量大需要分步读取以免爆内存的情况下，TestDriverClass也可以位于根节点。
- TestContainerClass是负责承接TestDriverClass和TestExecutorClass的节点。类比为树的话，它是树的中间节点。TestContainerClass不执行具体的测试也不负责测试调度，只负责处理中间过程。它会包含数据指针，但既不负责数据初始化也不负责数据清理。
- TestExecutorClass是具体执行测试的节点，或者类比为树的话是叶子节点。这类节点只负责执行**单个**测试，不应当预期使用它进一步调度其他测试。

在执行测试之前，用户需要：
1. 定义自定义的子类继承对应需要的父类
2. 在子类里继承父类中对应的虚函数，并实现对应目标功能
3. 利用每一个父类中的加载函数将数据加载到需要的节点上，组织成树状结构

# 测试框架使用文档

## 基础用法
这里的基础用法以源码中`example/main.cpp`提供的代码为例。

假设我们希望测试一个解析`std::string`长度的函数，于是我们构建了20组不同的测试集，每个测试集包含了50个具有某种特定特征的`std::string`数据（在提供的example里数据是自动生成的，并没有提供什么特殊特征，这里只是举例说明）。我们希望通过一个测试框架来结构化地将这1000条测试数据组织起来并提供有意义的运行结果，则我们可以基于当前框架如此操作：

1. 测试框架是header-only形式提供的，这意味着我们不需要对测试框架进行任何提前编译链接等等操作。在测试主程序代码文件（或者其他有意义的测试启动文件）里添加对`FuzzTestSchema.h`的引用就可以使用测试框架提供的所有功能，如下：
    ```cpp
    #include "FuzzTestSchema.h"
    ```
    同时，如果使用`Makefile`，请在其中添加一条对`<克隆测试框架的根目录>/FuzzTestSchema/src/ include`的引用，例如
    ```Makefile
    -I<克隆测试框架的根目录>/FuzzTestSchema/src/include
    ```
    如果使用的是`cmake`，请添加

    ```cmake
    include_directories(<克隆测试框架的根目录>/FuzzTestSchema/src/include)
    ```
2. 首先，虽然并非强制需求，但强烈推荐在编写测试之前声明必要的类型别名。测试框架内提供了一个名为`NameType`的别名自`std::string`的类型，用作便利测试组名和各类tag的类型声明。在此之外，我们也可以继续定义类似别名。以给定的例子而言，我们的测试数据的类型是`std::string`，那么我们就可以声明：
    ```cpp
    using BaseType = std::string;
    ```
    向上一级，我们需要声明测试集的类型（按上文，每个测试集包含50个测试数据），在这里我们推荐（但不强制）的  声明是将一组测试数据打包在`std::vector`里，然后将这个`std::vector`和上文提及的`NameType`一并打    包到一个`std::pair`里，以同时包含测试数据和名称信息。我们可以按如下代码声明两个类型别名：   `ContainerDatatype`用来表示一组测试数据，`ContainerType`用来表示上文提到的同时包含测试数据和名称   信息的测试集。
    ```cpp
    using ContainerDatatype = std::vector<BaseType>;
    using ContainerType = std::pair<ContainerDatatype, NameType>;
    ```
    再向上一级，我们需要声明测试包的类型（按上文，每个测试包包含20个测试集）。推荐的定义仍然同上所述，为：
    ```cpp
    using DriverDatatype = std::vector<ContainerType>;
    using DriverType = std::pair<DriverDatatype, NameType>;
    ```
    在这里我们就会发现声明类型别名的优势，由于必需的层层嵌套的测试数据，越往上层的数据类型会越长越复杂。例如，如果我们展开这个DriverType的定义，它会看起来像这样：
    ```cpp
    std::pair<std::vector<std::pair<std::vector<std::string>, std::string>>, std::string>
    ```
    很显然，这样定义的可读性非常差。当然，我们也可以选择声明一些struct甚至class来替代单纯的别名，这在一 些需要更复杂测试逻辑（以及数据准备和预处理工作）的时候会更加灵活。碍于篇幅所限，这里便不再展开。

3. 我们需要对应数据层级基于对应的父类声明实际的测试类
   1. 首先我们关注最顶层的数据结构，也即`DriverType`。在这里我们需要通过某种方式读取或者生成出所需的测试数据，同时我们希望声明一个位于顶层（或者相对顶层）的调度器来调度进一步的测试。那么我们应当继承预定义的`TestDriverClass`来实现我们的需求，如下：
        ```cpp
        class ExampleTestDriverClass : public TestDriverClass
        ```
        在继承后我们需要实现其中需求的三个方法，分别为`SetUp()`，`TearDown()`和`RunTest()`。其中：
        1. `SetUp()`是用来读取（或者生成）对应的目标测试数据的。请注意，在得到数据后务必需要将其赋值给`this->dataPtr`，这是一个`void*`的指针。获取的数据如果是栈上存储的，那么请将其移至堆上，随后将指针赋值给`this->dataPtr`，否则脱离`SetUp()`函数作用域后，获取的数据会被自动清理回收，这将会导致读取未定义数据的问题；如果是堆上存储的只需要简单赋值即可。测试框架内我们提供了一个实用宏`DATA_PTR`来帮助简化代码编写，其使用示例如下：
            ```cpp
            DriverType* matrix = DATA_PTR(DriverType);
            ```
            `DATA_PTR`的作用是将`this->dataPtr`静态转为传入的类型指针，我们在使用的时候可以简单将其视为指向this.dataPtr的传入类型的指针。上面给定的示例代码展开后为：
           ```cpp
           DriverType* matrix = static_cast<DriverType*>(this->dataPtr);
           ```
            具体的完整代码示例请参考示例代码。
        2. `TearDown()`的作用是在当前测试类的所有子测试全部运行完成之后清理上文`SetUp()`生成的数据指针。我们需要且只需要负责`SetUp()`中生成的数据指针（也就是`this->dataPtr`）的清理，不需要也不应当额外清理框架内其他指针，它们是由框架代码负责回收和维护状态的。回收方法和前文创建类似，可以参考如下示例代码：
            ```cpp
            delete DATA_PTR(DriverType);
            ```
        3. `RunTest()`的作用是调度和执行下层的测试。在其中我们需要声明一个循环来遍历上文生成的`std::vector`，解析出测试名和对应的子集测试信息，随后声明一个继承自`TestContainerClass`的子类，传入数据指针，并执行其预定义的构造函数，传入测试组名，最后调用其`ProceedTest()`函数并将得到的结果使用`this->testResult.appendSubTestResult()`函数附加到当前测试结果中。最后，返回`this->testResult`即可。各类额外的前后处理会自动执行，无须额外声明。上面提到的继承自`TestContainerClass`的子类将会在下面具体说明。
    2. 其次，我们关注中间层的测试调度。在这里我们已经接收了来自`ExampleTestDriverClass`提供的单个测试集的数据（这里预期数据类型为`ContainerType`），并且我们希望对这个数据进行进一步的遍历以将实际的测试用例传递给最下层的具体执行测试的代码。首先和上文一样，我们先要声明一下继承关系：
        ```cpp
        class ExampleTestContainerClass : public TestContainerClass
        {
        public:
            using TestContainerClass::TestContainerClass;
        ...
        }
        ```
        其中，`using TestContainerClass::TestContainerClass;`继承了来自父类`TestContainerClass`中预定义的构造函数。
        
        在这个类里我们需要执行的操作比较简单，仅需重载`RunTest()`函数即可。具体的重载方式和上文`ExampleTestDriverClass`几乎一致，同样也不需要额外声明各类前后处理。在声明中我们需要调用的下层类是继承自`TestExecutorClass`的子类，关于`TestExecutorClass`将会在下面介绍。当然，这里是可以继续引用其他`TestContainerClass`的，调用方法和`TestDriverClass`没有什么本质区别，除了不需要进行数据的准备和清理。这里不再就其赘述
    3. 最后，我们关注最底层的实际的测试执行者。到这一步我们已经把数据拆解到底层的单一测试样例了。对于这个需求，我们需要从`TestExecutorClass`继承子类并实现其方法。与`TestContainerClass`类似，我们不需要考虑数据的生成和清理，只需要重载`RunTest()`函数即可。在`RunTest()`里也需要像`TestContainerClass`一样将数据指针解析出来，随后自由调用需要测试的函数或者功能即可。框架提供了一些实用函数来进行测试，可以通过`this->testResult.assert...`来进行调用。其文档将会附在用法文档后。
 4. 我们需要声明一个`main`函数。在`main`函数里，我们一般只需要传入固定的两行代码即可：
    ```cpp
    ExampleTestDriverClass rootClass;
    rootClass.ProceedTest("ShowCase");
    ```
    第一句声明一个`ExampleTestDriverClass`的实例，调用默认构造函数即可。第二句启动测试并传入顶层测试集名称。
