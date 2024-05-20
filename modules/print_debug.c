#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/bpf.h>


#define OFFSET 0x215U

static struct kprobe kp = {

        .symbol_name = "bpf_trampoline_update",
        .offset = OFFSET,

};

static void __kprobes handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags){

        printk("Debug Probe is working!\n");
        int image_size = (int)regs->r14;

        struct bpf_tramp_image *img = (struct bpf_tramp_image*)(regs->r12 );
        uint8_t *im = (uint8_t*)img->image;
		struct bpf_trampoline *tr = (struct bpf_trampoline*)(regs->r15);

        printk("Image Size: %d\n", image_size);
        printk("Pointer to Image: %p\n", img);
        printk("Pointer to actual Image: %p\n", im);
        printk("Trampoline Flags: %x\n", tr->flags);


        for(int i = 8; i < image_size; i+=8){
                printk("%3d : %02x %02x %02x %02x %02x %02x %02x %02x\n", i - 8, im[i-8], im[i-7], im[i-6], im[i-5], im[i-4], im[i-3], im[i-2], im[i-1]);
        }

        switch(image_size % 8){

                case 1:
                        printk("%3d : %02x\n", image_size - 1, im[image_size - 1]);
                        break;
                case 2:
                        printk("%3d : %02x %02x\n", image_size - 2, im[image_size -2], im[image_size - 1]);
                        break;
                case 3:
                        printk("%3d : %02x %02x %02x\n", image_size - 3, im[image_size -3], im[image_size -2], im[image_size - 1]);
                        break;
                case 4:
                        printk("%3d : %02x %02x %02x %02x\n", image_size - 4, im[image_size -4], im[image_size -3], im[image_size -2], im[image_size - 1]);
                        break;
                case 5:
                        printk("%3d : %02x %02x %02x %02x %02x\n", image_size - 5, im[image_size -5], im[image_size -4], im[image_size -3], im[image_size -2], im[image_size - 1]);
                        break;
                case 6:
                        printk("%3d : %02x %02x %02x %02x %02x %02x\n", image_size - 6, im[image_size -6], im[image_size -5], im[image_size -4], im[image_size -3], im[image_size -2], im[image_size - 1]);
                        break;
                case 7:
                        printk("%3d : %02x %02x %02x %02x %02x %02x %02x\n", image_size - 7, im[image_size -7], im[image_size -6], im[image_size -5], im[image_size -4], im[image_size -3], im[image_size -2], im[image_size - 1]);
                        break;
 
		}
	
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
          
