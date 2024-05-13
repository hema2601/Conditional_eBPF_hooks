#include <linux/types.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
    
char __license[] __attribute__((section("license"), used)) = "GPL";
SEC("kprobe/napi_gro_receive")
int BPF_KPROBE(kprobe_napi_gro_receive){
    unsigned long ts;
    
  //  bpf_printk("Hello!");

    ts = bpf_ktime_get_ns();

    return 0;
}


/*
SEC("kprobe/do_execve")
int BPF_KPROBE(kprobe_do_execve, struct filename *filename) {
   struct data_t data = {}; 

   bpf_probe_read_kernel(&data.message, sizeof(data.message), kprobe_msg); 

   data.pid = bpf_get_current_pid_tgid() >> 32;
   data.uid = bpf_get_current_uid_gid() & 0xFFFFFFFF;

   bpf_get_current_comm(&data.command, sizeof(data.command));
   const char *name = BPF_CORE_READ(filename, name);
   bpf_probe_read_kernel(&data.path, sizeof(data.path), name);

   bpf_printk("%s: filename->name: %s", kprobe_msg, name);
   
   bpf_perf_event_output(ctx, &output, BPF_F_CURRENT_CPU, &data, sizeof(data));
   return 0;   
}*/
