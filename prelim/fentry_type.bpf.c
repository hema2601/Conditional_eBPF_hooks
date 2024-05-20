#include "vmlinux.h"

//#include <linux/types.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
    
char __license[] __attribute__((section("license"), used)) = "GPL";
SEC("fentry/__netif_receive_skb_core")
int BPF_PROG(fentry___netif_receive_skb_core){
    unsigned long ts;

    ts = bpf_ktime_get_ns();
    
//    bpf_printk("Sampled!");

    return 0;
}
