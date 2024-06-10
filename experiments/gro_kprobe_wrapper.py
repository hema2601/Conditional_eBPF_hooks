from bcc import BPF #1
from bcc.utils import printb

b = BPF(src_file="gro_kprobe.bpf.c") #3
b.attach_kprobe(event="napi_gro_complete", fn_name="kprobe_napi_gro_complete") #5

try:
    b.trace_print() #6
except KeyboardInterrupt: #7
    print("End")

d.detach_kprobe(event="napi_gro_complete", fn_name="kprobe_napi_gro_complete")

