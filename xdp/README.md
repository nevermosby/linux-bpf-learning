# XDP BPF 程序示例

## 相关博客地址
- 中文版：[https://davidlovezoe.club/bpf-xdp-101](https://davidlovezoe.club/bpf-xdp-101)

## 运行第一个XDP程序

1. 编写代码，文件名`xdp_drop_world.c`
   ```C
    #include <linux/bpf.h>
    int main() {
      // 意思是无论什么网络数据包，都drop丢弃掉
      return XDP_DROP;
    }
    ```
2. 编译代码，生成obj文件
   ```bash
   clang -O2 -target bpf -c xdp_drop_all.c -o xdp_drop_all.o
   ```
3. attach xdp 程序到主机网卡上
   ```bash
   # 通过ip link查询主机上的可用网卡设备名称，一般可以选取本机对外可以访问的IP所在的网卡设备
   ip link set dev [network-device-name] xdp obj xdp_drop_all.o sec .text
   ```
4. 测试
   - 使用`tcpdump`命令监测网络数据包传输情况
   - 使用`ping`命令创造测试网络数据包
5. 从主机网卡上detach xdp 程序
   ```bash
   ip link set dev [network-device-name] xdp off
   ```
- 完整Demo视频

  [![xdp-bpf-demo](https://img.youtube.com/vi/GD6pJLPd08U/0.jpg)](https://www.youtube.com/watch?v=GD6pJLPd08U)

- 小结
  
  这是一个非常简单XDP程序，使用了`xdpgeneric`模式attach到了主机网卡上，以丢弃任何传给这张网卡的网络数据包。

## 基于Docker运行第二个XDP程序

基于Nginx服务运行一个Docker容器，对外提供http服务。编写一个XDP程序，丢弃所有基于TCP协议的数据包。

1. 安装Docker，并启动一个web服务的容器
   ```bash
   docker run -d -p 80:80 --name=nginx-xdp nginx:alpine
   ```
2. 编写XDP代码
   
   ```c
   #include <linux/bpf.h>
   #include <linux/in.h>
   #include <linux/if_ether.h>
   #include <linux/ip.h>

   #define SEC(NAME) __attribute__((section(NAME), used))

   SEC("drop_tcp")
   int dropper(struct xdp_md *ctx) {
      int ipsize = 0;

      void *data = (void *)(long)ctx->data;
      void *data_end = (void *)(long)ctx->data_end;

      struct ethhdr *eth = data;
      ipsize = sizeof(*eth);
      struct iphdr *ip = data + ipsize;
      ipsize += sizeof(struct iphdr);

      if (data + ipsize > data_end) {
        return XDP_PASS;
      }

      // 判断是否该数据包是否基于TCP协议
      if (ip->protocol == IPPROTO_TCP) {
        // 丢弃该数据包
        return XDP_DROP;
      }

      return XDP_PASS;
    }

    char _license[] SEC("license") = "GPL";
   ```
3. 编译程序
   ```bash
   clang -O2 -target bpf -c xdp-drop-tcp.c -o xdp-drop-tcp.o
   ```
4. 找到docker容器在主机侧的veth pair网卡
   ```bash
   sandkey=$(docker inspect nginx-xdp -f "{{.NetworkSettings.SandboxKey}}")
   mkdir -p /var/run/netns
   ln -s $sandkey /var/run/netns/httpserver
   ip netns exec httpserver ip a
   > ip a | grep veth:
   20: veth5722074@if19: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP group default
   ```
5. 为主机侧的veth pair网卡 attach/detach XDP程序
   ```bash
   ip link set dev veth5722074 xdp obj xdp-drop-tcp.o sec drop_tcp
   ip link set dev veth5722074 xdp off
   ```
- 完整Demo视频
  - 从容器外访问NGINX http服务
 
   [![xdp-bpf-docker-ingress-demo](https://img.youtube.com/vi/SFDIsDoJG60/0.jpg)](https://www.youtube.com/watch?v=SFDIsDoJG60)

  - 从容器内访问某个http服务
  
   [![xdp-bpf-docker-engress-demo](https://img.youtube.com/vi/9O6PBnkxMOM/0.jpg)](https://www.youtube.com/watch?v=9O6PBnkxMOM)

  
- 小结

  从上面的例子可以看到XDP BPF程序只会对传给目标网卡的数据包进行丢弃，不会影响从目标网卡出去的数据包，也就是只影响ingress流量。那么能不能控制egress流量呢？

## 测试xdp下的ingress和egress
**说明**：
curl命令的`--dns-server`参数能帮助curl使用自定义的域名解析服务器，否则无法解析域名，只能访问IP。
这个参数在很多Linux发行版的curl里面没有编译进去，需要自行重新编译curl，带上该参数。可以参照这篇文章自定义curl命令：https://davidlovezoe.club/build-curl-from-source

- ingress
  ```bash
  curl localhost
  ```
- egress
  ```bash
  # curl-new是自己编译的版本，支持--dns-server参数
  # 下面这行是指定自己编译出来curl命令的lib库，否则无法使用curl
  export LD_LIBRARY_PATH=/usr/local/curl/fromsource/lib
  ip netns exec httpserver curl-new --dns-servers 8.8.8.8 www.baidu.com
  ```
