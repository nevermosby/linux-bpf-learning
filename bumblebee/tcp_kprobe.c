#include "vmlinux.h"
#include "bpf/bpf_helpers.h"
#include "bpf/bpf_core_read.h"
#include "bpf/bpf_tracing.h"
#include "solo_types.h"

// 1. Change the license if necessary 
char __license[] SEC("license") = "Dual MIT/GPL";

struct event_t {
	ipv4_addr daddr;
	u32 pid;
} __attribute__((packed));

struct dimensions_t {
	ipv4_addr daddr;
} __attribute__((packed));

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 8192);
	__type(key, struct dimensions_t);
	__type(value, u64);
} connection_count SEC(".maps.counter");

// This is the definition for the global map which both our
// bpf program and user space program can access.
// More info and map types can be found here: https://www.man7.org/linux/man-pages/man2/bpf.2.html
struct {
	__uint(max_entries, 1 << 24);
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__type(value, struct event_t);
} events SEC(".maps.print");

SEC("kprobe/tcp_v4_connect")
int BPF_KPROBE(tcp_v4_connect, struct sock *sk, struct sockaddr *uaddr) {
	// Init event pointer	
	struct event_t *event;
	struct dimensions_t hash_key = {};
	__u32 daddr;
	u64 counter;
	u64 *counterp;

	// read in the destination address
	struct sockaddr_in *usin = (struct sockaddr_in *)uaddr;
	daddr = BPF_CORE_READ(usin, sin_addr.s_addr);

	// Reserve a spot in the ringbuffer for our event
	event = bpf_ringbuf_reserve(&events, sizeof(struct event_t), 0);
	if (!event) {
		return 0;
	}
	// 3. set data for our event
	event->pid = bpf_get_current_pid_tgid();
	event->daddr = daddr;
	// submit the event (this makes it available for consumption)
	bpf_ringbuf_submit(event, 0);

	// increment the counter for this address
	hash_key.daddr = daddr;
	counterp = bpf_map_lookup_elem(&connection_count, &hash_key);
	if (counterp) {
		__sync_fetch_and_add(counterp, 1);
	} else {
		// we may miss N events, where N is number of CPUs. We may want to 
		// fix this for prod, by adding another lookup/update calls here.
		// we skipped these for brevity
		counter = 1;
		bpf_map_update_elem(&connection_count, &hash_key, &counter, BPF_NOEXIST);
	}

	return 0;
}