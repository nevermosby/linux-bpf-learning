# libbpf编程

原生[libbpf库](https://github.com/libbpf/libbpf)是C语言写的。随着bpf热度持续高涨，其他语言实现的bpf库也逐渐遍地开花。

## golang实现bpf库
- [gobpf](https://github.com/iovisor/gobpf): iovisor官方出品，可惜目前已经不活跃了
- [libbpfgo](https://github.com/aquasecurity/libbpfgo):通过go实现了一个基于C语言版本的libbpf的封装
- [ebpf](https://github.com/cilium/ebpf): 由Cilium和Cloudflare共同维护的一个纯Go库，它将所有的bpf系统调用抽象在一个原生Go接口里面。




