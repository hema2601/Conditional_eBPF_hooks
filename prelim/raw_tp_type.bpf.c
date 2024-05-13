#include <linux/types.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
    
char __license[] __attribute__((section("license"), used)) = "GPL";
SEC("raw_tracepoint/napi_gro_receive_entry")
int tp_napi_gro_receive_entry(void *ctx){
    unsigned long ts;
    
   // bpf_printk("Hello!");

    ts = bpf_ktime_get_ns();

    return 0;
}
