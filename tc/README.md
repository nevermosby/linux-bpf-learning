## tc learning
1. man手册
   1. man tc
   2. man tc-bpf
```bash
# attach it to a device with tc as:

  tc qdisc add dev eth0 clsact
  tc filter add dev eth0 egress  bpf da obj bar.o sec egress
  tc filter add dev eth0 ingress bpf da obj foo.o sec ingress
  tc filter show dev eth0 egress/ingress

# replace the bpf with
  tc filter replace dev eth0 egress bpf da obj tcp_ack.o
```

