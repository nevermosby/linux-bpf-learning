#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

/*
 * define map without btf
struct bpf_map_def SEC("maps") cnt = {
        .type = BPF_MAP_TYPE_ARRAY,
        .key_size = sizeof(__u32),
        .value_size = sizeof(long),
        .max_entries = 2,
};
*/

// define map with btf
struct {
        __uint(type, BPF_MAP_TYPE_ARRAY);
        __type(key, __u32);
        __type(value, long);
        __uint(max_entries, 2);
} cnt SEC(".maps");

/*
another define example
struct {
	__u32 type;
	__u32 max_entries;
	__u32 map_flags;
	__u32 key_size;
	__u32 value_size;
} stackmap SEC(".maps") = {
	.type = BPF_MAP_TYPE_STACK_TRACE,
	.max_entries = 128,
	.map_flags = BPF_F_STACK_BUILD_ID,
	.key_size = sizeof(__u32),
	.value_size = PERF_MAX_STACK_DEPTH * sizeof(struct bpf_stack_build_id),
};
*/

SEC("xdp_count")
int xdp_count_prog(struct xdp_md *ctx)
{
        void *data_end = (void *)(long)ctx->data_end;
        void *data = (void *)(long)ctx->data;
        __u32 ipv6_key = 0;
        __u32 ipv4_key = 1;
        long *value;
        __u16 h_proto;
        struct ethhdr *eth = data;
        if (data + sizeof(struct ethhdr) > data_end)
                return XDP_DROP;
        
        h_proto = eth->h_proto;
        if (h_proto == htons(ETH_P_IPV6)) {
                value = bpf_map_lookup_elem(&cnt, &ipv6_key);
                if (value)
                        *value += 1;
                return XDP_PASS;
        }
        value = bpf_map_lookup_elem(&cnt, &ipv4_key);
        if (value)
            *value += 1;
        return XDP_PASS;

}

char _license[] SEC("license") = "GPL";