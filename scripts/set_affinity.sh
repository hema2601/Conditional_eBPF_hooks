#!/bin/bash

source /usr/sbin/common_irq_affinity.sh

intf=${1:-ens6}
target=${2:-3}


echo "Disabling irqbalance service..."
service irqbalance stop

echo "Redirecting interrupts associated with $intf to CPU#$target"

for i in $( get_irq_list $intf );
do
    echo $((2**$target)) > /proc/irq/$i/smp_affinity;
    cat /proc/irq/$i/smp_affinity;
done 
