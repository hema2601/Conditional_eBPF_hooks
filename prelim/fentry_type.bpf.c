#include "vmlinux.h"

//#include <linux/types.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
    
char __license[] __attribute__((section("license"), used)) = "GPL";
SEC("fentry/napi_gro_receive")
int BPF_PROG(fentry_napi_gro_receive){
    bpf_printk("Hello!");
    //unsigned long ts;

    //ts = bpf_ktime_get_ns();

    return 0;
}
