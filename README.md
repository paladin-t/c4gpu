# The C4GPU (C for GPU) System

**Copyright (C) 2017 [Wang Renxin](https://linkedin.com/in/wangrenxin). All rights reserved.**

[Email](mailto:hellotony521@qq.com)

## Introduction; 简介

This project is the runtime part of the C4GPU system, which evaluates with massive data efficiently on GPU. Let's use C4GRT for short. It implements GPGPU with [Transform Feedback](https://www.khronos.org/opengl/wiki/Transform_Feedback) and programming with [GLSL](https://en.wikipedia.org/wiki/GLSL).

此项目是 C4GPU 系统的运行时部分，其作用为在 GPU 上对大量数据做高效运算。下文用 C4GRT 代指。利用 [Transform Feedback](https://www.khronos.org/opengl/wiki/Transform_Feedback) 和 [GLSL](https://en.wikipedia.org/wiki/GLSL) 实现 GPGPU。

## Compatibility; 兼容性

TODO : WRITE ME

## Performance; 性能

There are various profiling programs in the `test` directory.

在 `test` 文件夹下有多个性能测试程序。

### 1. C4GRT

Run the `test/c4grt_eval.exe` to profile with C4GRT. I used on of my other project [MY-BASIC](https://github.com/paladin-t/my_basic) for a scripting driven purpose.

运行 `test/c4grt_eval.exe` 进行 C4GRT 测试。我使用了我的另一个项目 [MY-BASIC](https://github.com/paladin-t/my_basic) 作为脚本驱动。

* It accepts a `.bas` file as optional argument, uses `prog.bas` as default;
* A `.bas` operates the C4GRT system, and evaluates using a vertex shader with prepared data;
* It's possible to modify the `.bas` and `.vert` vertex shader files to profile.

* 它接受一个 `.bas` 文件作为可选参数，缺省使用 `prog.bas`；
* `.bas` 脚本操作 C4GRT 系统，并使用一个 vertex shader 来对准备好的数据求值；
* 可对 `.bas` 和 `.vert` vertex shader 文件做测试修改。

### 2. C

Run the `test/c_eval.exe` to profile with native C. The source code file is `test/c_eval/c_eval.c`.

运行 `test/c_eval.exe` 进行本地 C 测试。其源码在 `test/c_eval/c_eval.c`。

### 3. C#

Run the `test/cs_eval.exe` to profile with C#.

运行 `test/cs_eval.exe` 进行 C# 测试。

* It accepts a `.cs` file as optional argument, uses `prog.cs` as default;
* It compiles and invokes the `.cs` source code on the fly;
* It's possible to midify the `.cs` file to profile.

* 它接受一个 `.cs` 文件作为可选参数，缺省使用 `prog.cs`；
* 它会动态编译执行 `.cs` 源代码；
* 可对 `.cs` 文件做测试修改。

TODO : ADD RESULT AND CHART

## Principles; 原理

### 1. Runtime; 运行时

#### 1.1 Data types; 数据类型

It's able to pass following data types.

可传递以下类型的数据。

* Basic data types;
* Texture data;
* Customized struct.

* 基础数据类型；
* 纹理数据；
* 自定义结构体。

#### 1.2 Work flow; 工作流程

Shader computation

Shader 计算

1. Creates context, binds shader
2. Input data
3. Computes shadering
4. Output data
5. Clears context

1. 创建上下文，绑定 Shader
2. 输入数据
3. 计算着色
4. 输出数据
5. 清理上下文

## How to use it as a lib; 如何使用库文件

TODO : WRITE ME

## Code at a glance; 示例

Read the `test/test/bas` and `test/test.vert` for a quick tutorial.

阅读 `test/test/bas` 以及 `test/test.vert` 作为快速入门。

TODO : WRITE MORE
