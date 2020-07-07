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

## 参考材料
