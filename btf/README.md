# BTF(BPF Type Format)入门

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

### 对比使用前后的差别

## BTF工作机制

## 联合BPF CO-RE机制

## 参考链接
- https://www.containiq.com/post/btf-bpf-type-format 