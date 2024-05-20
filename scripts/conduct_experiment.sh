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
	if [ "$prog_name" == "sampled_fentry_type" ]
	then
		#make clean -C $current_path/../modules -f Makefile
		#make -C $current_path/../modules -f Makefile
		insmod $current_path/../modules/insert_sampled.ko
		insmod $current_path/../modules/set_srate.ko
    		$current_path/insert_bpf_prog.sh fentry_type > /dev/null
	
	elif [ "$prog_name" == "sampled_fexit_type" ]
	then
		#make clean -C $current_path/../modules -f Makefile
		#make -C $current_path/../modules -f Makefile
		insmod $current_path/../modules/insert_sampled.ko
		insmod $current_path/../modules/set_srate.ko
    		$current_path/insert_bpf_prog.sh fexit_type > /dev/null

	else
    	$current_path/insert_bpf_prog.sh $prog_name > /dev/null
	fi
fi


taskset -c $iperf_core iperf3 -s -1 -J & ssh $remote_client_addr 'iperf3 -c 10.0.0.4 > /dev/null'

if [ "$prog_name" != "none" ]
then
	if [ "$prog_name" == "sampled_fentry_type" ]
	then
		rmmod insert_sampled
		rmmod set_srate
	elif [ "$prog_name" == "sampled_fexit_type" ]
	then
		rmmod insert_sampled
		rmmod set_srate
	fi 

    $current_path/remove_bpf_prog.sh > /dev/null
fi

$current_path/restore_affinity.sh $intf > /dev/null




