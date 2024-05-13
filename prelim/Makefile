
TARGETS = tp_type kprobe_type raw_tp_type fentry_type 

BPFTOOL = /usr/local/sbin/bpftool
PATH_TO_VMLINUX = /sys/kernel/btf/vmlinux

all: $(TARGETS)
.PHONY: all

$(TARGETS): %: %.bpf.o 

%.bpf.o: %.bpf.c vmlinux.h
	clang \
	    -target bpf \
		-I/usr/include/$(shell uname -m)-linux-gnu \
		-g \
	    -O2 -o $@ -c $<


vmlinux.h:
	$(BPFTOOL) btf dump file $(PATH_TO_VMLINUX) format c > vmlinux.h

clean: 
	- rm *.bpf.o
	- rm -f /sys/fs/bpf/tp_type 
	- rm -f /sys/fs/bpf/raw_tp_type 
	- rm -f /sys/fs/bpf/kprobe_type
	- rm -f /sys/fs/bpf/fentry_type
