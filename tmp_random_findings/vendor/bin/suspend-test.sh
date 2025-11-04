#!/bin/sh

setenforce 0
stop
#stop tvserver
stop vendor.hwcomposer-2-2
echo wakelock > /sys/power/wake_lock
svp-suspend

