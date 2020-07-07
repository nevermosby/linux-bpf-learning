#include <linux/types.h>

// define the struct for the key of bpf map
struct pair {
  __u32 src_ip;
  __u32 dest_ip;
};

struct stats {
  __u64 tx_cnt; // the sending request count
  __u64 rx_cnt; // the received request count
  __u64 tx_bytes; // the sending request bytes
  __u64 rx_bytes; // the sending received bytes
};