
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/bpf.h>


u32 sampling_rate = 10;

static struct kprobe kp1 = {

        .symbol_name = "bpf_trampoline_link_prog",
        .offset = 0x0,

};

static void __kprobes handler_post1(struct kprobe *p, struct pt_regs *regs, unsigned long flags){

        struct bpf_tramp_link *link = (struct bpf_tramp_link*)regs->di;

        printk("Setting Sampling Rate!\n");

        link->link.prog->aux->sampling_rate = sampling_rate;

        return;
}




static int __init kprobe_init(void){
        int ret;
        kp1.post_handler = handler_post1;

        ret = register_kprobe(&kp1);
        if(ret < 0){
                printk(KERN_ERR "KRPOBE MOD: registering the kprobe failed: %d\n", ret);
        }
        return 0;
}

static void __exit kprobe_exit(void){
        unregister_kprobe(&kp1);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");


