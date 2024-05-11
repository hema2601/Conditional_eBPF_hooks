#!/bin/bash

intf=${1:-ens6}


echo "Restoring interrupts of $intf"
/usr/sbin/set_irq_affinity.sh $intf

echo "Reenabling irqbalance service..."
service irqbalance start

