# bpftrace入门（In Progress）

## 监控bpftrace调用bpf()系统调用
```shell
> strace -e bpf /usr/bin/bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[comm] = count(); }'
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_ARRAY, key_size=4, value_size=4, max_entries=1, map_flags=0, inner_map_fd=0, map_name="", map_ifindex=0, btf_fd=0, btf_key_type_id=0, btf_value_type_id=0, btf_vmlinux_value_type_id=0}, 120) = 3
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_PERCPU_HASH, key_size=16, value_size=8, max_entries=4096, map_flags=0, inner_map_fd=0, map_name="@", map_ifindex=0, btf_fd=0, btf_key_type_id=0, btf_value_type_id=0, btf_vmlinux_value_type_id=0}, 120) = -1 EINVAL (Invalid argument)
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_PERCPU_HASH, key_size=16, value_size=8, max_entries=4096, map_flags=0, inner_map_fd=0, map_name="", map_ifindex=0, btf_fd=0, btf_key_type_id=0, btf_value_type_id=0, btf_vmlinux_value_type_id=0}, 120) = 3
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_PERF_EVENT_ARRAY, key_size=4, value_size=4, max_entries=2, map_flags=0, inner_map_fd=0, map_name="printf", map_ifindex=0, btf_fd=0, btf_key_type_id=0, btf_value_type_id=0, btf_vmlinux_value_type_id=0}, 120) = 4
Attaching 1 probe...
bpf(BPF_MAP_UPDATE_ELEM, {map_fd=4, key=0x7ffeae5cd18c, value=0x7ffeae5cd190, flags=BPF_ANY}, 120) = 0
bpf(BPF_MAP_UPDATE_ELEM, {map_fd=4, key=0x7ffeae5cd18c, value=0x7ffeae5cd190, flags=BPF_ANY}, 120) = 0
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=27, insns=0x7fa74c50c000, license="GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(5, 13, 19), prog_flags=0, prog_name="sys_enter", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=0, func_info_rec_size=0, func_info=NULL, func_info_cnt=0, line_info_rec_size=0, line_info=NULL, line_info_cnt=0, attach_btf_id=0, attach_prog_fd=0}, 120) = 9
```