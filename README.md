# The C4GPU Runtime

**Copyright (C) 2017 [Wang Renxin](https://github.com/paladin-t). All rights reserved.**

[Email](mailto:hellotony521@qq.com)

[![MIT license](http://img.shields.io/badge/license-MIT-brightgreen.svg)](http://opensource.org/licenses/MIT)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)

<hr />

[简体中文](#简介)

## Introduction

### 1. Gist

Welcome to the C4GPU Runtime. The C4GPU Runtime accelerated computing library is a free, general purpose, open source library that simplifies the process of developing software that targets parallel and massively parallel computing benefits by hardware acceleration with GPU. Let's call it C4GRT for short. It implements a GPGPU method with [Transform Feedback](https://www.khronos.org/opengl/wiki/Transform_Feedback) and programming with [GLSL](https://en.wikipedia.org/wiki/GLSL). With this library, you don't need to care about how to manipulate GPU API. The only thing you need to do is use the C API, and write standard GLSL shader for evaluation.

### 2. Why use GLSL for GPGPU

There are some other GPGPU techniques available. But GLSL is almost the sole option if you'd like to write legal cross platform GPU program, although it's designed as a graphics shadering language.

C4GRT is an abstraction layer of GPGPU with OpenGL.

## Compatibility

The repository contains some precompiled libraries as follow.

Platform | Path | Usage
----- | ----- | -----
Windows | [lib/x64/c4grt.dll](lib/x64/c4grt.dll)/[lib/x86/c4grt.dll](lib/x86/c4grt.dll) | Dynamic libraries
macOS | [lib/macos/c4grt_macos.framework](lib/macos/c4grt_macos.framework) | Framework
iOS | [lib/ios/c4grt_ios.framework](lib/ios/c4grt_ios.framework) | Framework
Linux | TO BE ADDED | -
Android | TO BE ADDED | -
Raspberry Pi | TO BE ADDED | -

You have to use a C++11 compatible compiler to make a build manually.

## Performance

There are various profiling programs in the [`test`](test) directory.

This repository only contains test programs for Windows for the moment. It requires VC++ 2015 x64 runtime to run C++ based programs, please use [`redist/vc_redist.x64.exe`](https://c4gpu.github.io/c4gpu_runtime/redist/vc_redist.x64.exe) to install it.

### 1. C4GRT

Run the `test/c4grt_eval.exe` to profile with C4GRT. I used one of my other project [MY-BASIC](https://github.com/paladin-t/my_basic) for a scripting driven purpose.

* It accepts a `.bas` file as optional argument, uses [`prog.bas`](test/prog.bas) as default;
* A `.bas` operates the C4GRT system, and evaluates using a vertex shader with prepared data;
* It's possible to modify the `.bas` and `.vert` vertex shader files to profile.

### 2. C

Run the `test/c_eval.exe` to profile with native C. The source code file is [`test/c_eval/c_eval.c`](test/c_eval/c_eval.c).

### 3. C Sharp

Run the `test/cs_eval.exe` to profile with C#.

* It accepts a `.cs` file as optional argument, uses [`prog.cs`](test/prog.cs) as default;
* It compiles and invokes the `.cs` source code on the fly;
* It's possible to midify the `.cs` file to profile.

### 4. Comparisons

I've tested the profiler programs on a desktop computer as follow.

	Intel Core i5-6500 CPU, 3.2GHz
	DDR4 2.4GHz RAM, 8GB
	NVIDIA GeForce GTX950 GPU, 2GB

They result approximately as follow.

Host | Program | Time cost
----- | ----- | -----
C4GRT | [prog.bas](test/prog.bas)/[prog.vert](test/prog.vert) | 230ms
C | - | 2.5min
C# | [prog.cs](test/prog.cs) | 4.8min

The result may be different according to specific hardwares. But without doubt the performance of the GPGPU solution stands out remarkably.

## Principles

### 1. Runtime

#### 1.1 Data types

It's able to pass the following data types.

* Basic data types;
* Texture data;
* Customized struct (TO BE IMPLEMENTED).

#### 1.2 Work flow

Shader computation.

1. Creates context, binds shader;
2. Inputs data;
3. Computes shadering;
4. Outputs data;
5. Clears context.

## How to use it as a lib

The C4GRT is implemented with C++11, and has exposed an ANSI C interface. You can see [`src/runtime`](src/runtime) for the implementation source code. But for most cases, you may only include the [`include/c4g_runtime.h`](include/c4g_runtime.h) header file to use the library.

## Code at a glance

Read the [`test/test.bas`](test/test.bas) and [`test/test.vert`](test/test.vert) for a quick tutorial. You could also read the [`src/shell/c4g.cpp`](src/shell/c4g.cpp) to see how to use it with C/C++ programs.

A common workflow works as follow.

~~~~~~~~~~bas
rt = runtime() ' Creates a C4GRT instance.
rt.begin_proc() ' Begins processing, actives current context.
p0 = rt.add_pass() ' Adds an evaluation pass.
rt.use_gpu_program_file(p0, "prog.vert", "o0") ' Uses a GPU program, with Transform Feedback varyings.

rt.prepare_buffers(p0, 0, len(ina0), len(outa0)) ' Prepares buffers.
rt.prepare_uniform(p0, una0) ' Prepares uniform data.
rt.prepare_in(p0, ina0) ' Prepares input data.
rt.prepare_out(p0, outa0) ' Prepares output buffer.

rt.compute(p0) ' Does evaluation.

rt.map_out(p0) ' Maps output data.
rt.finish() ' Finishes.
rt.end_proc() ' Ends processing.
~~~~~~~~~~

It's also possible to link several evaluation passes into an evaluation sequence, in which a pass will use some of the output data of previous pass as its input. Use the follow code to link two passes.

~~~~~~~~~~bas
p0 = rt.add_pass() ' Adds the first pass.
p1 = rt.add_pass(p0) ' Adds the second pass, and uses p0 as its previous.
rt.set_pass_pipe(p0, true, "o0", "v0", "o1", "v1") ' Sets how to pass output data from p0 to p1 as input.
~~~~~~~~~~

In the above code, the first parameter of `set_pass_pipe` specifies the ahead pass, and the second boolean parameter indicates whether the output data would flow to its following pass. The rest variadic arguments list in a form of key-value pairs tells the naming rules of data flow. Here `p0`'s output `o0` links to `p1`'s input `v0`, `p0`'s `o1` links to `p1`'s `v1`.

Without invoking `set_pass_pipe`, the linked passes work as a sequence one by one, but without output to input data flow.

It's also possible to partially set some of the data by piping, and the rest by common prepared input buffers.

## Limitations

It requires OpenGL 2.0 or OpenGL ES 3.0 to use the Transform Feedback. Some GPUs didn't implement texture sampler in vertex shader.

<hr />

[English](#introduction)

## 简介

### 1. 主旨

欢迎来到 C4GPU Runtime。C4GPU Runtime 计算加速库是一个免费、通用、开源的程序库，得益于 GPU 硬件加速，用以简化面向并行及大规模并行计算的软件开发过程。下文均用 C4GRT 代指本项目。其利用 [Transform Feedback](https://www.khronos.org/opengl/wiki/Transform_Feedback) 和 [GLSL](https://en.wikipedia.org/wiki/GLSL) 实现了 GPGPU 的一种方法。使用本库你不需要关心如何操作 GPU API。你唯一需要关注的就是调用 C API，以及编写标准 GLSL 着色程序用以计算。

### 2. 为何使用 GLSL 做 GPGPU

能用来做 GPGPU 的技术有很多。但是如果你想写合法的跨平台 GPU 程序，GLSL 差不多是唯一的选择了，尽管其设计初衷是作为图形着色语言。

C4GRT 是针对 GPGPU 的 OpenGL 调用的一层抽象。

## 兼容性

本源码仓库包含一些预编译库文件，如下。

平台 | 路径 | 使用
----- | ----- | -----
Windows | [lib/x64/c4grt.dll](lib/x64/c4grt.dll)/[lib/x86/c4grt.dll](lib/x86/c4grt.dll) | 动态链接库
macOS | [lib/macos/c4grt_macos.framework](lib/macos/c4grt_macos.framework) | Framework
iOS | [lib/ios/c4grt_ios.framework](lib/ios/c4grt_ios.framework) | Framework
Linux | TO BE ADDED | -
Android | TO BE ADDED | -
Raspberry Pi | TO BE ADDED | -

如需手动构建，你需要使用一个兼容 C++11 的编译器。

## 性能

在 [`test`](test) 文件夹下有多个性能测试程序。

本源码仓库暂时只提供针对 Windows 平台的测试程序。基于 C++ 的程序运行需要 VC++ 2015 x64 运行时支持，请使用 [`redist/vc_redist.x64.exe`](https://c4gpu.github.io/c4gpu_runtime/redist/vc_redist.x64.exe) 进行安装。

### 1. C4GRT

运行 `test/c4grt_eval.exe` 进行 C4GRT 测试。我使用了我的另一个项目 [MY-BASIC](https://github.com/paladin-t/my_basic) 作为脚本驱动。

* 程序接受一个 `.bas` 文件作为可选参数，缺省使用 [`prog.bas`](test/prog.bas)；
* `.bas` 脚本操作 C4GRT 系统，并使用一个 vertex shader 来对准备好的数据求值；
* 可对 `.bas` 和 `.vert` vertex shader 文件做测试修改。

### 2. C

运行 `test/c_eval.exe` 进行本地 C 测试。其源码在 [`test/c_eval/c_eval.c`](test/c_eval/c_eval.c)。

### 3. C Sharp

运行 `test/cs_eval.exe` 进行 C# 测试。

* 程序接受一个 `.cs` 文件作为可选参数，缺省使用 [`prog.cs`](test/prog.cs)；
* 程序会动态编译执行 `.cs` 源代码；
* 可对 `.cs` 文件做测试修改。

### 4. 对比

本人在一台台式机上运行效率测试程序，配置如下。

	Intel Core i5-6500 CPU, 3.2GHz
	DDR4 2.4GHz RAM, 8GB
	NVIDIA GeForce GTX950 GPU, 2GB

大致运行结果如下。

宿主 | 程序 | 耗时
----- | ----- | -----
C4GRT | [prog.bas](test/prog.bas)/[prog.vert](test/prog.vert) | 230ms
C | - | 2.5min
C# | [prog.cs](test/prog.cs) | 4.8min

不同硬件可能导致结果不同。但毫无疑问 GPGPU 解决方案的效率明显胜出。

## 原理

### 1. 运行时

#### 1.1 数据类型

可传递以下类型的数据。

* 基础数据类型；
* 纹理数据；
* 自定义结构体（待实现）。

#### 1.2 工作流程

Shader 计算。

1. 创建上下文，绑定 Shader；
2. 输入数据；
3. 计算着色；
4. 输出数据；
5. 清理上下文。

## 如何使用库文件

C4GRT 使用 C++11 实现，并且其接口为 ANSI C 导出。如需查看实现源码，请看 [`src/runtime`](src/runtime) 文件夹。但对于大多数使用情景来说，包含 [`include/c4g_runtime.h`](include/c4g_runtime.h) 头文件就足够了。

## 示例

阅读 [`test/test.bas`](test/test.bas) 以及 [`test/test.vert`](test/test.vert) 作为快速入门。你也可以查看 [`src/shell/c4g.cpp`](src/shell/c4g.cpp) 以了解如何在 C/C++ 程序中使用它。

一个典型工作流程如下。

~~~~~~~~~~bas
rt = runtime() ' 创建一个 C4GRT 实例。
rt.begin_proc() ' 开始处理，激活当前上下文。
p0 = rt.add_pass() ' 添加一个计算 pass。
rt.use_gpu_program_file(p0, "prog.vert", "o0") ' 使用某个 GPU 程序，附带 Transform Feedback 传出命名。

rt.prepare_buffers(p0, 0, len(ina0), len(outa0)) ' 准备 buffer。
rt.prepare_uniform(p0, una0) ' 准备 uniform 数据。
rt.prepare_in(p0, ina0) ' 准备输入数据。
rt.prepare_out(p0, outa0) ' 准备输出缓存。

rt.compute(p0) ' 进行计算。

rt.map_out(p0) ' 映射输出数据。
rt.finish() ' 完成。
rt.end_proc() ' 完成处理。
~~~~~~~~~~

同时系统允许把多个计算 pass 连接成一个计算序列，其中某一 pass 会使用其前一 pass 的某些输出数据作为输入。使用如下代码来连接两个 pass。

~~~~~~~~~~bas
p0 = rt.add_pass() ' 添加第一个 pass。
p1 = rt.add_pass(p0) ' 添加第二个 pass，且使用 p0 作为其前置 pass。
rt.set_pass_pipe(p0, true, "o0", "v0", "o1", "v1") ' 设置如何将 p0 输出传递给 p1 作为输入。
~~~~~~~~~~

在上述代码中，`set_pass_pipe` 的第一个参数指定首 pass，第二个 boolean 参数指定其输出数据会流向其后继 pass。其余以“键值对”存在的变长参数列表为数据流向定义命名规则。在这里 `p0` 的输出 `o0` 连接到 `p1` 的输入 `v0`，`p0` 的 `o1` 连接到 `p1` 的 `v1`。

如果没有调用 `set_pass_pipe`，则已连接到 pass 只是一个接一个的被计算，并不会有输出到输入的数据流向。

程序中同样支持部分指定数据流向管道，其余数据使用常规输入缓存。

## 限制

使用 Transform Feedback 特性要求 OpenGL 2.0 或 OpenGL ES 3.0 及以上版本。某些 GPU 不允许在 vertes shader 里使用纹理采样。
