# The C4GPU (C for GPU) System

**Copyright (C) 2017 [Wang Renxin](https://linkedin.com/in/wangrenxin). All rights reserved.**

[Email](mailto:hellotony521@qq.com)

## Introduction; 简介

### 1. Gist; 主旨

This project is the runtime part of the C4GPU system, which evaluates with massive data efficiently on GPU. Let's use C4GRT for short. It implements a GPGPU method with [Transform Feedback](https://www.khronos.org/opengl/wiki/Transform_Feedback) and programming with [GLSL](https://en.wikipedia.org/wiki/GLSL). With this library, you don't need to care about how to manipulate GPU API. The only thing you need to do is use the C API, and write standard GLSL shader for evaluation.

此项目是 C4GPU 系统的运行时部分，其作用为在 GPU 上对大量数据做高效运算。下文均用 C4GRT 代指。其利用 [Transform Feedback](https://www.khronos.org/opengl/wiki/Transform_Feedback) 和 [GLSL](https://en.wikipedia.org/wiki/GLSL) 实现了 GPGPU 的一种方法。使用本库你不需要关心如何操作 GPU API。你唯一需要关注的就是调用 C API，以及编写标准 GLSL 着色程序用以计算。

### 2. Why GLSL for GPGPU; 为何使用 GLSL 做 GPGPU

There are some other GPGPU techniques available. But GLSL is almost the sole option if you'd like to write cross platform GPU program, and legally, although it's designed as graphics shader.

能用来做 GPGPU 的技术有很多。但是如果你想写跨平台，且合法的 GPU 程序，GLSL 差不多是唯一的选择了，尽管其设计初衷是做图形学着色。

## Compatibility; 兼容性

This repository only contains libraries for Windows for the moment.

本 repo 暂时只提供针对 Windows 平台的库文件。

TODO : ADD BIN FOR MORE OS

## Performance; 性能

There are various profiling programs in the `test` directory.

在 `test` 文件夹下有多个性能测试程序。

### 1. C4GRT

Run the `test/c4grt_eval.exe` to profile with C4GRT. I used one of my other project [MY-BASIC](https://github.com/paladin-t/my_basic) for a scripting driven purpose.

* It accepts a `.bas` file as optional argument, uses `prog.bas` as default;
* A `.bas` operates the C4GRT system, and evaluates using a vertex shader with prepared data;
* It's possible to modify the `.bas` and `.vert` vertex shader files to profile.

运行 `test/c4grt_eval.exe` 进行 C4GRT 测试。我使用了我的另一个项目 [MY-BASIC](https://github.com/paladin-t/my_basic) 作为脚本驱动。

* 程序接受一个 `.bas` 文件作为可选参数，缺省使用 `prog.bas`；
* `.bas` 脚本操作 C4GRT 系统，并使用一个 vertex shader 来对准备好的数据求值；
* 可对 `.bas` 和 `.vert` vertex shader 文件做测试修改。

### 2. C

Run the `test/c_eval.exe` to profile with native C. The source code file is `test/c_eval/c_eval.c`.

运行 `test/c_eval.exe` 进行本地 C 测试。其源码在 `test/c_eval/c_eval.c`。

### 3. C Sharp

Run the `test/cs_eval.exe` to profile with C#.

* It accepts a `.cs` file as optional argument, uses `prog.cs` as default;
* It compiles and invokes the `.cs` source code on the fly;
* It's possible to midify the `.cs` file to profile.

运行 `test/cs_eval.exe` 进行 C# 测试。

* 程序接受一个 `.cs` 文件作为可选参数，缺省使用 `prog.cs`；
* 程序会动态编译执行 `.cs` 源代码；
* 可对 `.cs` 文件做测试修改。

### 4. Comparisons; 对比

I've tested the profiler programs on a desktop computer as follow.

	Indel Core i5-6500 CPU, 3.2GHz
	DDR4 2.4GHz RAM, 8 GB
	NVIDIA GeForce GTX950 GPU, 2GB

They result approximately as follow.

Host | Program | Time cost
----- | ----- | -----
C4GRT | prog.bas/prog.vert | 230ms
C | - | 2.5min
C# | prog.cs | 4.8min

No doubt the performance of the GPGPU solution stands out remarkably.

本人在一台台式机上运行效率测试程序，配置如下。

	Indel Core i5-6500 CPU, 3.2GHz
	DDR4 2.4GHz RAM, 8 GB
	NVIDIA GeForce GTX950 GPU, 2GB

大致运行结果如下。

宿主 | 程序 | 耗时
----- | ----- | -----
C4GRT | prog.bas/prog.vert | 230ms
C | - | 2.5min
C# | prog.cs | 4.8min

毫无疑问 GPGPU 解决方案的效率明显胜出。

## Principles; 原理

### 1. Runtime; 运行时

#### 1.1 Data types; 数据类型

It's able to pass the following data types.

* Basic data types;
* Texture data;
* Customized struct (TO BE IMPLEMENTED).

可传递以下类型的数据。

* 基础数据类型；
* 纹理数据；
* 自定义结构体（待实现）。

#### 1.2 Work flow; 工作流程

Shader computation.

1. Creates context, binds shader;
2. Inputs data;
3. Computes shadering;
4. Outputs data;
5. Clears context.

Shader 计算。

1. 创建上下文，绑定 Shader；
2. 输入数据；
3. 计算着色；
4. 输出数据；
5. 清理上下文。

## How to use it as a lib; 如何使用库文件

The C4GRT is implemented with C++11, and has exposed an ANSI C interface. You can see `src/runtime` for the implementation source code. But for most cases, you may only include the `include/c4g_runtime.h` header file to use the library.

C4GRT 使用 C++11 实现，并且其接口为 ANSI C 导出。如需查看实现源码，请看 `src/runtime` 文件夹。但对于大多数使用情景来说，包含 `include/c4g_runtime.h` 头文件就足够了。

## Code at a glance; 示例

Read the `test/test.bas` and `test/test.vert` for a quick tutorial. You could also read the `src/shell/c4g.cpp` to see how to use it with C/C++ programs.

A common workflow works as follow.

	rt = runtime() ' Creates a C4GRT instance.
	p0 = rt.add_pass() ' Adds an evaluation pass.
	rt.use_gpu_program_file(p0, "prog.vert", "o0") ' Uses a GPU program, with Transform Feedback varyings.

	rt.prepare_buffers(p0, 0, len(ina0), len(outa0)) ' Prepares buffers.
	rt.prepare_uniform(p0, una0) ' Prepares uniform data.
	rt.prepare_in(p0, ina0) ' Prepares input data.
	rt.prepare_out(p0, outa0) ' Prepares output buffer.

	rt.compute(p0) ' Does evaluation.

	rt.map_out(p0) ' Maps output data.
	rt.finish() ' Finishes.

It's also possible to link several evaluation passes into an evaluation sequence, in which a pass will use some of the output data of previous pass as its input. Use the follow code to link two passes.

	p0 = rt.add_pass() ' Adds the first pass.
	p1 = rt.add_pass(p0) ' Adds the second pass, and uses p0 as its previous.
	rt.set_pass_pipe(p0, true, "o0", "v0", "o1", "v1") ' Sets how to pass output data from p0 to p1 as input.

阅读 `test/test.bas` 以及 `test/test.vert` 作为快速入门。你也可以查看 `src/shell/c4g.cpp` 以了解如何在 C/C++ 程序中使用它。

一个典型工作流程如下。

	rt = runtime() ' 创建一个 C4GRT 实例。
	p0 = rt.add_pass() ' 添加一个计算 pass。
	rt.use_gpu_program_file(p0, "prog.vert", "o0") ' 使用某个 GPU 程序，附带 Transform Feedback 传出命名。

	rt.prepare_buffers(p0, 0, len(ina0), len(outa0)) ' 准备 buffer。
	rt.prepare_uniform(p0, una0) ' 准备 uniform 数据。
	rt.prepare_in(p0, ina0) ' 准备输入数据。
	rt.prepare_out(p0, outa0) ' 准备输出缓存。

	rt.compute(p0) ' 进行计算。

	rt.map_out(p0) ' 映射输出数据。
	rt.finish() ' 完成。

同时系统允许把多个计算 pass 连接成一个计算序列，其中某一 pass 会使用其前一 pass 的某些输出数据作为输入。使用如下代码来连接两个 pass。

	p0 = rt.add_pass() ' 添加第一个 pass。
	p1 = rt.add_pass(p0) ' 添加第二个 pass，且使用 p0 作为其前置 pass。
	rt.set_pass_pipe(p0, true, "o0", "v0", "o1", "v1") ' 设置如何将 p0 输出传递给 p1 作为输入。

## limitations; 限制

It requires OpenGL 2.0 or OpenGL ES 3.0 to use the Transform Feedback.

使用 Transform Feedback 特性要求 OpenGL 2.0 或 OpenGL ES 3.0 及以上版本。
