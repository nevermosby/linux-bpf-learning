## TC BPF 程序示例

## 相关博客地址
- 中文版：[https://davidlovezoe.club/bpf-tc-101](https://davidlovezoe.club/bpf-tc-101)

## 推荐阅读tc man手册
tc相关的man手册写得很完整，推荐大家能通读一遍。
- man tc
- man tc-bpf
  
## 运行第一个TC程序

1. 设计代码
   
    XDP是RX链路的第一层，TC是TX链路上的第一层，那么我们就设计一个同时使用这两个hook的程序，让他们一起发光发热，这次我们把流量粒度控制得更细点，实现把双向TCP流量都drop掉。这次拿容器实例作为流控目标。在实验环境上通过docker run运行一个Nginx服务。
    ```bash
    docker run -d -p 80:80 --name=nginx-xdp-tc nginx:alpine

    # 查看 veth-pair
    > ip a | grep veth
    6: veth09e1d2e@if5: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP group default
    ```

2. 编写代码

    源代码在[tc-xdp-drop.tcp.c](./tc-xdp-drop-tcp.c)

3. 编译代码
    ```bash
    clang -I ./headers/ -O2 -target bpf -c tc-xdp-drop-tcp.c -o tc-xdp-drop-tcp.o
    ```
4. 加载代码

    ```bash
    # 最开始的状态
    > tc qdisc show dev veth09e1d2e
    qdisc noqueue 0: root refcnt 2
    # 创建clsact
    > tc qdisc add dev veth09e1d2e clsact
    # 再次查看，观察有什么不同
    > tc qdisc show dev veth09e1d2e
    qdisc noqueue 0: root refcnt 2
    qdisc clsact ffff: parent ffff:fff1
    # 加载TC BPF程序到容器的veth网卡上
    > tc filter add dev veth09e1d2e egress bpf da obj tc-xdp-drop-tcp.o sec tc
    # 再次查看，观察有什么不同
    > tc qdisc show dev veth09e1d2e
    qdisc noqueue 0: root refcnt 2
    qdisc clsact ffff: parent ffff:fff1
    > tc filter show dev veth09e1d2e egress
    filter protocol all pref 49152 bpf chain 0
    filter protocol all pref 49152 bpf chain 0 handle 0x1 tc-xdp-drop-tcp.o:[tc] direct-action not_in_hw id 24 tag 9c60324798bac8be jited
    ```
5. 运行效果
   
    Demo 视频:
    
    [![tc-xdp-drop-tcp-docker-demo](https://img.youtube.com/vi/NSoK9rCuGP8/0.jpg)](https://www.youtube.com/watch?v=NSoK9rCuGP8)