#!/bin/bash
current_path=/home/hema/conditional_eBPF_hooks/scripts

set -e


bpftool=/usr/local/sbin/bpftool

echo "clear tracing..."

echo 0 > /sys/kernel/tracing/tracing_on
echo 0 > /sys/kernel/tracing/events/enable
echo "" > /sys/kernel/tracing/trace

echo "Make programs..."

make clean -C $current_path/../prelim -f Makefile



