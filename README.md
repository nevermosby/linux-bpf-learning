# 学习Linux BPF XDP编程

## 相关博文参考
- 中文版：https://davidlovezoe.club/wordpress/archives/tag/xdp

## 环境准备
- Linux操作系统，推荐使用最新稳定内核版本.
  
  本人自己的实验环境是`Ubuntu 18.04`标准版vagrant虚拟机，内核版本为`4.15.0`。可以从这里下载该vagrant虚拟机环境，已安装bcc工具集合：
  
  > 下载链接: https://pan.baidu.com/s/11dsEU6Yk6KGDGNor-fbsgQ 提取码: qvhc。
  > 使用方式可以参考[这篇文章](https://davidlovezoe.club/ebpf-learning-bcc-intro)。
  
  以下命令如无特殊说明，均在Ubuntu环境下测试执行。

- 预装clang、LLVM、iproute2、libelf-dev
  ```shell
  # for ubuntu
  apt install clang llvm libelf-dev
  # test clang
  clang -v
  # test llvm
  llc --version
  # test iproute2
  ip link
  ```

## 运行第一个XDP程序

1. 编写代码,文件名`xdp_drop_all.c`
   ```C
    #include <linux/bpf.h>
    int main() {
      # 意思是无论什么网络数据包，都drop丢弃掉
      return XDP_DROP;
    }
    ```
2. 编译代码，生成obj文件
   ```shell
   clang -O2 -target bpf -c xdp_drop_all.c -o xdp_drop_all.o
   ```
3. attach xdp 程序到主机网卡上
   ```shell
   # 通过ip link查询主机上的可用网卡设备名称，一般可以选取本机对外可以访问的IP所在的网卡设备
   ip link set dev [network-device-name] xdp obj xdp_drop_all.o sec .text
   ```
4. 测试
   - 使用`tcpdump`命令监测网络数据包传输情况
   - 使用`ping`命令创造测试网络数据包
5. 从主机网卡上detach xdp 程序
   ```shell
   ip link set dev [network-device-name] xdp off
   ```

## 完整Demo视频
[![xdp-bpf-demo](https://img.youtube.com/vi/GD6pJLPd08U/0.jpg)](https://www.youtube.com/watch?v=GD6pJLPd08U)


## 基于Docker运行第二个XDP程序

```C
#include <linux/bpf.h>

#define SEC(NAME) __attribute__((section(NAME), used))

SEC("xdp")
int xdp_drop_all(struct xdp_md *ctx)
{
	return XDP_DROP;
}
```

## 参考材料
- https://davidlovezoe.club/wordpress/archives/tag/bpf