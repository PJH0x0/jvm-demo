# 项目介绍

这是一个基于《自己动手写Java虚拟机》的C++实现，用于学习Java虚拟机的实现原理。

# 项目构建

## 依赖

- cmake version >= 3.11.0
- C++11
- jdk 1.8

## 构建

```bash
mkdir build
cmake -B build
cmake --build build
```

## 运行

确定jdk 1.8已经安装，然后执行，并且找到对应的jre路径，替换下面的路径

```bash
# /usr/lib/jvm/java-8-openjdk-amd64/jre需要替换成对应的jre路径
# -cp tests/javasample/需要替换成对应的class文件路径，这里是javasample目录
./build/java -Xjre /usr/lib/jvm/java-8-openjdk-amd64/jre -cp tests/javasample/ com.sample.ch06.MyObject
```

## coredump

由于项目不是特别稳定，可能会出现崩溃，可以通过下面的命令查看coredump

```bash
ulimit -c unlimited
```

Linux下默认的coredump地址是/var/lib/apport/coredump, macos下默认的coredump地址是/cores/

```bash
# 查看coredump，/cores/core.1234需要替换成对应的coredump路径
lldb -c /cores/core.1234
```

# 项目结构

## libs

用于存放第三方库，目前有glog和ziplib，glog用于日志输出，ziplib用于读取jar文件

## src

用于存放实现的源代码

- classfile: 解析class文件，包括常量池、字段、方法、属性等
- classpath: 查找class文件，包括目录、zip文件、jar文件等，同时也包括读取class文件到内存
- instructions: 实现字节码指令
- native : 实现本地方法
- rtda : 实现运行时数据区，包括线程、栈、帧、局部变量表、操作数栈等
- rtda/heap: 实现堆内存，包括对象、数组、方法区等

## tests

用于存放测试代码

- javasample: 用于测试书中java demo
- googletest: gtest三方库代码
- src: 单元测试代码
