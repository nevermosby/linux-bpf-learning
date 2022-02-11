// +build ignore
#include "hellokprobe.h"

// Example: tracing a message on a kprobe
SEC("kprobe/sys_execve")
int hello(void *ctx)
{
    bpf_printk("Hello from kprobe");
    return 0;
}