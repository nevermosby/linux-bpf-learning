# 学习Linux BPF/eBPF 编程

打造学习BPF知识的中文社区。学习计划如下:
![bpf-learning-path](https://davidlovezoe.club/wordpress/wp-content/uploads/2020/06/eBPF-learning-002-2048x1528.png)

## 相关博文参考
- 中文版：https://davidlovezoe.club/wordpress/archives/tag/bpf

## 实验环境准备
- Linux操作系统，推荐使用最新稳定内核版本.
  
  本人自己的实验环境是`Ubuntu 18.04`标准版vagrant虚拟机，内核版本为`4.15.0`。可以从这里下载该vagrant虚拟机环境，已安装bcc工具集合：
  
  > 下载链接: https://pan.baidu.com/s/11dsEU6Yk6KGDGNor-fbsgQ 提取码: qvhc。
  > 使用方式可以参考[这篇文章](https://davidlovezoe.club/ebpf-learning-bcc-intro)。
  
  以下命令如无特殊说明，均在Ubuntu环境下测试执行。

- 预装clang、LLVM、iproute2、libelf-dev
  ```bash
  # for ubuntu
  apt install clang llvm libelf-dev iproute2
  # test clang
  clang -v
  # test llvm
  llc --version
  # test iproute2
  ip link
  ```
- `bpftool`命令行安装说明

  下载Linux内核源码，进行本地编译。
  ```bash
  # 确认内核版本
  uname -r
  # 找到对应内核版本的源代码
  apt-cache search linux-source
  apt install linux-source-5.3.0
  apt install libelf-dev

  cd /usr/src/linux-source-5.3.0
  tar xjf linux-source-5.3.0.tar.bz2
  cd linux-source-5.3.0/tools
  make -C  bpf/bpftool/
  cd bpf/bpftool/
  ./bpftool prog/net
  ```

## 目录说明
- [bpf知识整理](./bpf/README.md)
  - 经典文章翻译
  - 编译Linux内核所有bpf示例代码
- [网络XDP编程](./xdp/README.md) 
- [网络TC编程](./tc/README.md)

## 常见问题Q&A
### 1. 'asm/type.h' file not found

- 错误现象

  在执行下面命令进行代码编译时，可能会遇到某些头文件找不到的错误：

  ```shell
  clang -I ./headers/ -O2 -target bpf -c tc-xdp-drop-tcp.c -o tc-xdp-drop-tcp.o

  In file included from tc-xdp-drop-tcp.c:2:
  In file included from /usr/include/linux/bpf.h:11:
  /usr/include/linux/types.h:5:10: fatal error: 'asm/types.h' file not found
  #include <asm/types.h>
          ^~~~~~~~~~~~~
  1 error generated.
  ```

- 原因分析

  在源代码文件中引用了某些系统目录（一般为`/usr/include/`）下的头文件，而这些头文件没有出现在目标路径下，导致编译失败。
  
  如上述问题中的asm相关文件，asm全称`Architecture Specific Macros`，直译过来“与机器架构相关的宏文件”，顾名思义它是跟机器架构密切相关的，不同的架构x86、x64、arm实现是不一样的，而操作系统并没有提供`/usr/include/asm/`这样通用的目录，只提供了具体架构相关的目录，如`/usr/include/x86_64-linux-gnu/asm/`，因此无法找到引用。

- 解决方案
  
  添加软链`/usr/include/asm/`，指向操作系统自带的asm目录：
  ```shell
  cd /usr/include
  ln -s ./x86_64-linux-gnu/asm asm
  ```

## 参考材料
