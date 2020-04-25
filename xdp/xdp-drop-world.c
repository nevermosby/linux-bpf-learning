#include <linux/bpf.h>

/*
 * Helper macro to place programs, maps, license in
 * different sections in elf_bpf file. Section names
 * are interpreted by elf_bpf loader.
 * You can either use the helper header file below
 * so that you don't need to defind it yourself:
 * #include <bpf/bpf_helpers.h> 
 */
#define SEC(NAME) __attribute__((section(NAME), used))

// entrance for this program
SEC("xdp")
int xdp_drop_the_world(struct xdp_md *ctx) {
    // 意思是无论什么网络数据包，都drop丢弃掉
    return XDP_DROP;
}

// for bpf verifier
char _license[] SEC("license") = "GPL";

// another simple version of this pragram
/*
#include <linux/bpf.h>
int xdp_drop_the_world() {
    return XDP_DROP;
}
*/