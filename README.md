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

## 概述

本测试框架提供了一种组织和执行测试的机制，它基于树状结构组织测试流程。主要包含三个类：`TestDriverClass`、`TestContainerClass` 和 `TestExecutorClass`。下面将详细介绍这些类的功能和使用方法。

## 测试框架组件介绍

### TestDriverClass

`TestDriverClass` 负责加载和释放数据，以及调度下层测试类执行测试。类比为一棵树的话，`TestDriverClass` 便是根节点，但它不一定需要位于最顶端。在一些测试量大需要分步读取以免爆内存的情况下，`TestDriverClass` 也可以位于树的中间位置。

#### 主要职责
- **初始化数据**：通过 `SetUp` 方法完成。
- **清理数据**：通过 `TearDown` 方法完成。
- **执行测试**：通过 `ProceedTest` 方法调度具体的测试逻辑。

#### 使用方法
1. 继承 `TestDriverClass`。
2. 实现 `SetUp` 方法，用于准备测试前的环境。
3. 实现 `TearDown` 方法，用于清理测试后的环境。
4. 实现 `RunTest` 方法，用于执行具体的测试逻辑。

### TestContainerClass

`TestContainerClass` 负责承接 `TestDriverClass` 和 `TestExecutorClass` 的节点。类比为树的话，它是树的中间节点。`TestContainerClass` 不执行具体的测试，只负责处理中间过程和调度。它会包含数据指针，但既不负责数据初始化也不负责数据清理。

#### 主要职责
- **管理子测试类**：通过 `sublevelTestClasses` 管理多个子测试类。
- **传递数据指针**：将数据指针传递给子测试类。

#### 使用方法
1. 继承 `TestContainerClass`。
2. 根据需要创建并管理子测试类。
3. 确保数据指针正确传递给子测试类。

### TestExecutorClass

`TestExecutorClass` 是具体执行测试的节点，类比为树的话是叶子节点。这类节点只负责执行单个测试，不应当预期使用它进一步调度其他测试。

#### 主要职责
- **执行单个测试**：通过 `ProceedTest` 方法执行具体的测试逻辑。
- **标记测试索引**：通过 `testIndex` 标识当前测试在测试序列中的位置。

#### 使用方法
1. 继承 `TestExecutorClass`。
2. 实现 `ProceedTest` 方法，用于执行具体的测试逻辑。
3. 使用 `testIndex` 来标识测试的位置。

### 使用辅助函数

1. **格式化文本输出**：
   - 使用 `printStyledText` 函数来打印带有颜色和样式的文本。此函数可以接受颜色、样式和是否换行等选项。

2. **追加格式化文本**：
   - 使用 `appendStyledText` 函数来在现有字符串末尾追加带有颜色和样式的文本。

3. **删除最后一行**：
   - 使用 `deleteLastLine` 函数来删除控制台上的最后一行文本。

4. **获取格式化文本**：
   - 使用 `getStyledText` 函数来获取带有颜色和样式的文本字符串。

## 注意事项

- 用户不应直接继承 `GenericTestClass` 类，而应继承其具体的子类，如 `TestExecutorClass` ， `TestContainerClass`和`TestDriverClass`。
- 在编写测试逻辑时，请确保正确处理测试结果，并适当地更新 `TestResult` 对象。
- 在使用格式化输出函数时，请注意终端是否支持 ANSI 转义码。