## tc learning
1. man手册
   1. man tc
   2. man tc-bpf
```bash

# compile
clang -I ./headers/ -O2 -target bpf -c layercoop.c -o layercoop.o
clang -I ./headers/ -O2 -target bpf -c tc-xdp-drop-tcp.c -o tc-xdp-drop-tcp.o

# attach is to a network device with ip:
  ip link set dev [network-device-name] xdp obj tc-xdp-drop-tcp.o sec xdp

# attach it to a device with tc:

  tc qdisc add dev eth0 clsact
  tc filter add dev eth0 egress bpf da obj tc-xdp-drop-tcp.o sec tc
  tc filter add dev eth0 ingress bpf da obj tc-xdp-drop-tcp.o sec ingress
  tc filter show dev eth0 egress/ingress

# replace the bpf with
  tc filter replace dev eth0 egress bpf da obj tcp_ack.o
  tc filter del dev eth0 egress

curl localhost
ip netns exec httpserver curl-new --dns-servers 8.8.8.8  www.baidu.com
ip netns exec httpserver curl-new 220.181.38.148

# remove
tc filter delete dev veth369809a egress
ip link set dev veth369809a xdp off

/bpftool map dump id 14

```