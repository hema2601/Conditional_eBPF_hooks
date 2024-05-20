#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/bpf.h>

static struct kprobe kp = {

        .symbol_name = "bpf_prog_load",
        .offset = 0x0,

};

static void __kprobes handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags){

        union bpf_attr *attr = (union bpf_attr*)regs->di;


        if(attr->expected_attach_type == BPF_TRACE_FENTRY){
                attr->expected_attach_type = BPF_TRACE_SAMPLED_FENTRY;
                printk("Changed en!\n");
        }
        if(attr->expected_attach_type == BPF_TRACE_FEXIT){
                attr->expected_attach_type = BPF_TRACE_SAMPLED_FEXIT;
                printk("Changed ex!\n");
        }

        printk("Probe1 is working!\n");
        return;
}


static int __init kprobe_init(void){
        int ret;
        kp.post_handler = handler_post;

        ret = register_kprobe(&kp);
        if(ret < 0){
                printk(KERN_ERR "KRPOBE MOD: registering the kprobe failed: %d\n", ret);
        }
        return 0;
}

static void __exit kprobe_exit(void){
        unregister_kprobe(&kp);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
          
