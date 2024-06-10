#!/bin/bash
current_path=/home/hema/Conditional_eBPF_hooks/scripts

set -e


prog_name=${1:-no}
bpftool=/usr/local/sbin/bpftool

if [ "$prog_name" = "no" ]
then
    echo "Error: Enter bpf program name!"
    exit
fi

echo "clear tracing..."

echo 0 > /sys/kernel/tracing/tracing_on
echo 0 > /sys/kernel/tracing/events/enable
echo "" > /sys/kernel/tracing/trace

echo "Make programs..."

make -C $current_path/../experiments -f Makefile

echo "Insert program $prog_name..."

$bpftool prog load $current_path/../experiments/$prog_name.bpf.o /sys/fs/bpf/$prog_name autoattach



echo "Reenabling tracing"
echo 1 > /sys/kernel/tracing/tracing_on


