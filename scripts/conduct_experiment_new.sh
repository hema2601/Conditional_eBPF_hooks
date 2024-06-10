#!/bin/bash

set -e

current_path=/home/hema/Conditional_eBPF_hooks/scripts
bpftool=/usr/local/sbin/bpftool
remote_client_addr=hema@115.145.178.16

prog_name=${1:-no}
intf=${2:-ens6}
iperf_core=${3:-2}
irq_core=${4:-3}



if [ "$prog_name" = "no" ]
then
    echo "Error: Enter bpf program name!"
    exit
fi


$current_path/set_affinity.sh $intf $irq_core > /dev/null


if [ "$prog_name" != "none" ]
then

	case $prog_name in

	fentry)
    		$current_path/../experiments/libbpf_bootstrap/fentry > /dev/null &

		;;
	fentry_10)
		insmod $current_path/../modules/insert_sampled.ko
		insmod $current_path/../modules/set_srate_10.ko
    		$current_path/../experiments/libbpf_bootstrap/fentry > /dev/null &

		;;

	fentry_100)
		insmod $current_path/../modules/insert_sampled.ko
		insmod $current_path/../modules/set_srate_100.ko
    		$current_path/../experiments/libbpf_bootstrap/fentry > /dev/null &

		;;

	fentry_1000)
		insmod $current_path/../modules/insert_sampled.ko
		insmod $current_path/../modules/set_srate_1000.ko
    		$current_path/../experiments/libbpf_bootstrap/fentry > /dev/null &
		;;
	
	sampled_fentry_10)
    		$current_path/../experiments/libbpf_bootstrap/sampled_fentry_10 > /dev/null &
		;;

	sampled_fentry_100)
    		$current_path/../experiments/libbpf_bootstrap/sampled_fentry_100 > /dev/null &
		;;

	sampled_fentry_1000)
    		$current_path/../experiments/libbpf_bootstrap/sampled_fentry_1000 > /dev/null &
		;;

	esac
	
	pid=$!

fi


taskset -c $iperf_core iperf3 -s -1 -J & ssh $remote_client_addr 'iperf3 -c 10.0.0.4 > /dev/null'



if [ "$prog_name" != "none" ]
then
    
	kill $pid

	case $prog_name in
	fentry_10)
		rmmod insert_sampled
		rmmod set_srate_10
		;;

	fentry_100)
		rmmod insert_sampled
		rmmod set_srate_100
		;;

	fentry_1000)
		rmmod insert_sampled
		rmmod set_srate_1000
		;;

	esac

fi

$current_path/restore_affinity.sh $intf > /dev/null




