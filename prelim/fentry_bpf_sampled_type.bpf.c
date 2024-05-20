#include "vmlinux.h"

//#include <linux/types.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, __u64);
    __type(value, __u64);
    __uint(max_entries, 64);
} counter_table SEC(".maps");;
    
char __license[] __attribute__((section("license"), used)) = "GPL";
SEC("fentry/__netif_receive_skb_core")
int BPF_PROG(fentry___netif_receive_skb_core){


	u64 uid;
	u64 counter = 0;
	u64 *p;
	uid = bpf_get_current_uid_gid() & 0xFFFFFFFF;
	
 	p = bpf_map_lookup_elem(&counter_table, &uid);	
	if (p != 0) {
		counter = *p;
	}else{

	//	bpf_printk("Failed!");
 		bpf_map_update_elem(&counter_table, &uid, &counter, BPF_ANY);

		return 0;
	}

	if(counter != 99){
		counter++;
	}else{
		//bpf_printk("Sampled!");
		unsigned long ts = bpf_ktime_get_ns(); 	
		counter = 0;
	}
		
 	bpf_map_update_elem(&counter_table, &uid, &counter, BPF_ANY);


	return 0; 
}
