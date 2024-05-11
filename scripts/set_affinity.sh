#!/bin/bash

source /usr/sbin/common_irq_affinity.sh

target=${1:-3}
intf=${2:-ens6}


echo "Disabling irqbalance service..."
service irqbalance stop

echo "Redirecting interrupts associated with $intf to CPU#$target"

for i in $( get_irq_list $intf );
do
    echo $((2**$target)) > /proc/irq/$i/smp_affinity;
    cat /proc/irq/$i/smp_affinity;
done 
