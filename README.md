# FuzzTestSchema
适用于C++的多层级FuzzTest框架

# 框架说明
- TestDriverClass是负责加载和释放数据，以及调度下层测试类执行测试的对象。类比为一棵树的话，TestDriverClass便是根节点，但TestDriverClass不一定需要位于最顶端，在一些测试量大需要分步读取以免爆内存的情况下，TestDriverClass也可以位于根节点。TestDriverClass构造时需要传入负责加载数据的函数指针以及必要的参数列表。
- TestContainerClass是负责承接TestDriverClass和TestExecutorClass的节点。类比为树的话，它是树的中间节点。TestContainerClass不执行具体的测试也不负责测试调度，只负责处理中间过程。它会包含数据指针，但既不负责数据初始化也不负责数据清理。
- TestExecutorClass是具体执行测试的节点，或者类比为树的话是叶子节点。这类节点只负责执行**单个**测试，不应当预期使用它进一步调度其他测试。

在执行测试之前，用户需要：
1. 定义自定义的子类继承对应需要的父类
2. 在子类里继承父类中对应的虚函数，并实现对应目标功能
3. 利用每一个父类中的加载函数将数据加载到需要的节点上，组织成树状结构

# 测试框架使用指南

## 概述

本测试框架旨在为用户提供一种简便的方式来组织和执行测试用例。框架支持不同层次的测试类，包括执行单一测试的类 (`TestExecutorClass`) 和包含多个子测试的容器类 (`TestContainerClass`)。此外，还提供了一系列辅助函数来帮助格式化输出和管理测试结果。

## 使用方法

### 创建测试执行者类 (`TestExecutorClass`)

1. **继承 `TestExecutorClass`**：
   - 创建一个新的类继承自 `TestExecutorClass`。
   
2. **实现 `ProceedTest` 方法**：
   - 实现 `ProceedTest` 方法来定义测试逻辑。此方法接收一个测试名称，并返回一个 `TestResult` 对象。

3. **构造函数初始化**：
   - 在构造函数中初始化测试所需的任何数据和索引。

### 创建测试容器类 (`TestContainerClass`)

1. **继承 `TestContainerClass`**：
   - 创建一个新的类继承自 `TestContainerClass`。

2. **实现 `ProceedTest` 方法**：
   - 实现 `ProceedTest` 方法来定义测试逻辑。此方法接收一个测试名称，并返回一个 `TestResult` 对象。

3. **构造函数初始化**：
   - 在构造函数中初始化是否为叶子节点的父节点，并初始化测试所需的任何数据。

### 运行测试

1. **设置测试**：
   - 在测试执行之前调用 `SetUp` 方法来准备测试环境。

2. **执行测试**：
   - 调用 `ProceedTest` 方法来执行测试。此方法会调用 `SetUp` 方法设置测试环境，然后运行实际的测试逻辑，并在完成后调用 `TearDown` 方法来清理环境。

3. **清理测试**：
   - 在测试执行之后调用 `TearDown` 方法来清理测试环境。

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

- 用户不应直接继承 `GenericTestClass` 类，而应继承其具体的子类，如 `TestExecutorClass` 或 `TestContainerClass`。
- 在编写测试逻辑时，请确保正确处理测试结果，并适当地更新 `TestResult` 对象。
- 在使用格式化输出函数时，请注意终端是否支持 ANSI 转义码。