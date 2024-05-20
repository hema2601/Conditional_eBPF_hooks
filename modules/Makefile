obj-m += insert_sampled.o set_srate.o print_debug.o

all:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
