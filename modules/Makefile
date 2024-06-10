obj-m += insert_sampled.o set_srate_10.o set_srate_100.o set_srate_1000.o print_debug.o

all:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
