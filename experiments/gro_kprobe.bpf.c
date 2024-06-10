#include "vmlinux.h"

//#include <linux/types.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
    
char __license[] __attribute__((section("license"), used)) = "GPL";
SEC("kprobe/napi_gro_complete")
int BPF_KPROBE(kprobe_napi_gro_complete, struct napi_struct *napi,  struct sk_buff *skb){

    struct napi_gro_cb *start = (struct napi_gro_cb*)(skb)->cb;

    void *end =  start + sizeof(struct napi_gro_cb);

    u16 count = 0;

    bpf_probe_read_kernel(&count, sizeof(u16), &(start->count));
 


    bpf_printk("KProbe: GRO accumulated %u packets!", count);

    return 0;
}
