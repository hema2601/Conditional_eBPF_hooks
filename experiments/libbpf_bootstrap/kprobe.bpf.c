// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/* Copyright (c) 2021 Sartura */
#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";


struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, int);
    __type(value, uint64_t);
    __uint(max_entries, 2);
} gro_map SEC(".maps");


SEC("kprobe/napi_gro_complete")
int BPF_KPROBE(kprobe_napi_gro_complete, struct napi_struct *napi, struct sk_buff *skb)
{

    struct napi_gro_cb *cb = (struct napi_gro_cb*)(skb)->cb;

    int cnt_idx = 0;
    int acc_idx = 1;
    u64 count = 0;
    u64 acc = 0;
    u64 *p;

    p = bpf_map_lookup_elem(&gro_map, &cnt_idx);

    if(p){
        count = *p;
    }

    if(count){
        p = bpf_map_lookup_elem(&gro_map, &acc_idx);
        if(p){
            acc = *p;
        }
    }else
        acc = 0;

    u16 pkts = 0;

    bpf_probe_read_kernel(&pkts, sizeof(u16), &(cb->count));

    count++;
    acc+=pkts;

    bpf_map_update_elem(&gro_map, &cnt_idx, &count, BPF_ANY);
    bpf_map_update_elem(&gro_map, &acc_idx, &acc, BPF_ANY);

    return 0;


}
