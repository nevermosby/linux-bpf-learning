# BTF(BPF Type Format)入门(In Progress)

## BTF诞生原因
先来看看[Linux内核社区是怎么说的](https://www.kernel.org/doc/html/latest/bpf/btf.html)：

> BTF (BPF Type Format) is the metadata format which encodes the debug info
related to BPF program/map. The name BTF was used initially to describe data
types. The BTF was later extended to include function info for defined
subroutines, and line info for source/line information.

> BTF（BPF类型格式）是一种元数据格式，对与BPF程序/map有关的调试信息进行编码。与BPF程序/map有关的元数据格式。BTF这个名字最初是用来描述数据类型的。后来，BTF被扩展到包括已定义的子程序的函数信息和源/行信息的行信息。

> The debug info is used for map pretty print, function signature, etc. The function signature enables better bpf program/function kernel symbol. The line info helps generate source annotated translated byte code, jited code and verifier log.

> 调试信息用于map pretty print、函数签名等。函数签名可以更好地实现bpf程序/函数的内核符号。行信息有助于生成源码注释的翻译字节码、jited代码和验证器日志。

### 是什么，不是什么
BTF (BPF Type Format) 是作为一个更通用，更详细的DWARF调试信息的替代品而创建的。BTF是一种节省空间，紧凑但仍具有足够表达能力的格式，可以描述C程序的所有类型信息
### 优势
- BTF类型信息，用于允许获取关于内核和BPF程序类型和代码的关键信息，进而为解决BPF CO-RE的其他难题提供了可能性
- 由于其简单性和使用的重复数据删除算法，与DWARF相比，BTF的大小可减少多达100倍。

## BTF的应用
1. 内核开启BTF
    启用CONFIG_DEBUG_INFO_BTF=y内核选项即可。内核本身可以使用BTF功能，用于增强BPF验证程序自身的功能。

2. 程序中声明使用BTF Map
    使用`SEC(.maps)`宏定义（是在这个[commit](https://github.com/libbpf/libbpf/commit/ec13b303499c881496116881784883c9e44e436b)中引入）

### 对比使用前后的差别

```s
> llvm-objdump -h btf_xdp_cnt.o 

btf_xdp_cnt.o:  file format elf64-bpf

Sections:
Idx Name             Size     VMA              Type
  0                  00000000 0000000000000000 
  1 .strtab          000000e3 0000000000000000 
  2 .text            00000000 0000000000000000 TEXT
  3 xdp_count        00000120 0000000000000000 TEXT
  4 .relxdp_count    00000020 0000000000000000 
  5 .maps            00000020 0000000000000000 DATA
  6 license          00000004 0000000000000000 DATA
  7 .debug_loc       0000017c 0000000000000000 DEBUG
  8 .debug_abbrev    00000119 0000000000000000 DEBUG
  9 .debug_info      0000028e 0000000000000000 DEBUG
 10 .rel.debug_info  000003d0 0000000000000000 
 11 .debug_str       000001b7 0000000000000000 DEBUG
 12 .BTF             000004d4 0000000000000000 
 13 .rel.BTF         00000020 0000000000000000 
 14 .BTF.ext         00000140 0000000000000000 
 15 .rel.BTF.ext     00000110 0000000000000000 
 16 .debug_frame     00000028 0000000000000000 DEBUG
 17 .rel.debug_frame 00000020 0000000000000000 
 18 .debug_line      00000146 0000000000000000 DEBUG
 19 .rel.debug_line  00000010 0000000000000000 
 20 .llvm_addrsig    00000003 0000000000000000 
 21 .symtab          00000150 0000000000000000
```

## BTF工作机制

1. 初始化机制
bpf_object__init_maps(https://github.com/libbpf/libbpf/blob/master/src/libbpf.c#L2568) -> 
bpf_object__init_user_btf_maps(https://github.com/libbpf/libbpf/blob/master/src/libbpf.c#L2516) -> 
bpf_object__init_user_btf_map ->
bpf_object__add_map: https://github.com/libbpf/libbpf/blob/master/src/libbpf.c#L1401 


## 联合BPF CO-RE机制

## 参考链接
- https://www.containiq.com/post/btf-bpf-type-format
- https://github.com/libbpf/libbpf/blob/master/src/btf.h 
- https://github.com/libbpf/libbpf/blob/master/src/libbpf.c#L2516
- https://github.com/libbpf/libbpf/blob/master/src/libbpf.c#L2579