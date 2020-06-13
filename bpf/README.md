# bpf知识整理

## 经典文章翻译
- [A thorough introduction to eBPF](https://davidlovezoe.club/ebpf-learning-001)
- [An introduction to the BPF Compiler Collection](https://davidlovezoe.club/ebpf-learning-bcc-intro)
- [Early packet drop — and more — with BPF](https://davidlovezoe.club/ebpf-learning-xdp-init-intro)

## 编译Linux内核源码bpf示例代码 samples/bpf 

0. 详细说明可以查看博文：[https://davidlovezoe.club/compile-bpf-examples](https://davidlovezoe.club/compile-bpf-examples)

1. 下载你的实验环境对应的内核版本的Linux内核源代码
2. 保证你的实验环境已经安装`clang`和`llvm`
    * clang >= version 3.4.0
    * llvm >= version 3.7.1
3. 编译bpf示例代码
```bash
# 切换到内核源代码根目录
cd linux_sourcecode/
# 生成内核编译时需要的头文件
make headers_install
# 生成.config文件为下面执行命令做准备
make menuconfig
# 使用make命令编译samples/bpf/目录下所有bpf示例代码，注意需要加上最后的/符号
make samples/bpf/ # or  make M=samples/bpf

# 本人的实验环境是Ubuntu 18.04 with 4.15.0内核，在执行上面的make命令时，
# 发生了以下的错误信息
...
In file included from ./tools/perf/perf-sys.h:9:0,
                 from samples/bpf/bpf_load.c:28:
./tools/perf/perf-sys.h: In function ‘sys_perf_event_open’:
./tools/perf/perf-sys.h:68:15: error: ‘test_attr__enabled’ undeclared (first use in this function)
  if (unlikely(test_attr__enabled))
               ^
./tools/include/linux/compiler.h:74:43: note: in definition of macro ‘unlikely’
 # define unlikely(x)  __builtin_expect(!!(x), 0)
                                           ^
./tools/perf/perf-sys.h:68:15: note: each undeclared identifier is reported only once for each function it appears in
  if (unlikely(test_attr__enabled))
               ^
./tools/include/linux/compiler.h:74:43: note: in definition of macro ‘unlikely’
 # define unlikely(x)  __builtin_expect(!!(x), 0)
                                           ^
In file included from samples/bpf/bpf_load.c:28:0:
./tools/perf/perf-sys.h:69:3: warning: implicit declaration of function ‘test_attr__open’ [-Wimplicit-function-declaration]
   test_attr__open(attr, pid, cpu, fd, group_fd, flags);
   ^~~~~~~~~~~~~~~
scripts/Makefile.host:107: recipe for target 'samples/bpf/bpf_load.o' failed
make[1]: *** [samples/bpf/bpf_load.o] Error 1
Makefile:1823: recipe for target 'samples/bpf/' failed
make: *** [samples/bpf/] Error 2

# 查看./tools/perf/perf-sys.h这个文件，发现报错的那一行是test开头
# 通过Google发现了内核大佬们的邮件来往：https://www.spinics.net/lists/netdev/msg608676.html
# 大佬建议由于是测试相关的代码，所以可以skip掉。
# 修改完的文件就是同目录下的perf-sys.h,请斟酌参考
# 重新运行一下命令
make samples/bpf/ # and it works
```

## bpftool cheetsheet
```bash

bpftool prog tracelog # short for “cat /sys/kernel/debug/tracing/trace_pipe”

```
