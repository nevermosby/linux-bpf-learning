#include <linux/bpf.h>
#include <linux/if_link.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <time.h>
#include "bpf_load.h"
#include <bpf/bpf.h>
#include "bpf_util.h"
#include "xdp_ip_tracker_common.h"

static int ifindex = 6; // target network interface to attach, you can find it via `ip a`
static __u32 xdp_flags = 0;

// unlink the xdp program and exit
static void int_exit(int sig)
{
    printf("stopping\n");
    set_link_xdp_fd(ifindex, -1, xdp_flags);
    exit(0);
}

// An XDP program which track packets with IP address
// Usage: ./xdp_ip_tracker
int main(int argc, char **argv)
{
    char *filename = "xdp_ip_tracker_kern.o";
    // change limits
    struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
    if (setrlimit(RLIMIT_MEMLOCK, &r))
    {
        perror("setrlimit(RLIMIT_MEMLOCK, RLIM_INFINITY)");
        return 1;
    }

    // load the bpf kern file
    if (load_bpf_file(filename))
    {
        printf("error %s", bpf_log_buf);
        return 1;
    }

    // confirm the bpf prog fd is available
    if (!prog_fd[0])
    {
        printf("load_bpf_file: %s\n", strerror(errno));
        return 1;
    }

    // add signal handlers
    signal(SIGINT, int_exit);
    signal(SIGTERM, int_exit);

    // link the xdp program to the network interface
    if (set_link_xdp_fd(ifindex, prog_fd[0], xdp_flags) < 0)
    {
        printf("link set xdp fd failed\n");
        return 1;
    }

    int result;
    struct pair next_key, lookup_key = {0, 0};
    struct stats value = {};
    while (1)
    {
        sleep(2);
        // retrieve the bpf map of statistics
        while (bpf_map_get_next_key(map_fd[0], &lookup_key, &next_key) != -1)
        {
            //printf("The local ip of next key in the map is: '%d'\n", next_key.src_ip);
            //printf("The remote ip of next key in the map is: '%d'\n", next_key.dest_ip);
            struct in_addr local = {next_key.src_ip};
            struct in_addr remote = {next_key.dest_ip};
            printf("The local ip of next key in the map is: '%s'\n", inet_ntoa(local));
            printf("The remote ip of next key in the map is: '%s'\n", inet_ntoa(remote));
            
            // get the value via the key
            // TODO: change to assert
            // assert(bpf_map_lookup_elem(map_fd[0], &next_key, &value) == 0)
            result = bpf_map_lookup_elem(map_fd[0], &next_key, &value);
            if (result == 0)
            {
                // print the value
                printf("rx_cnt value read from the map: '%llu'\n", value.rx_cnt);
                printf("rx_bytes value read from the map: '%llu'\n", value.rx_bytes);
            }
            else
            {
                printf("Failed to read value from the map: %d (%s)\n", result, strerror(errno));
            }
            lookup_key = next_key;
            printf("\n\n");
        }
        printf("start a new loop...\n");
        // reset the lookup key for a fresh start
        lookup_key.src_ip = 0;
        lookup_key.dest_ip = 0;
    }

    printf("end\n");
    // unlink the xdp program
    set_link_xdp_fd(ifindex, -1, xdp_flags);
    return 0;
}