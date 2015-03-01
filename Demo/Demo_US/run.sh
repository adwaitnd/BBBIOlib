#!/bin/bash


# Change cpu governor
echo "performance" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 

# Add uart overlay
CAPEMGR=$(cat /sys/devices/platform/bone_capemgr/slots | grep UART5)

#remove slot
#sudo sh -c "echo '-4' > /sys/devices/platform/bone_capemgr/slots"

if [ $? -eq 0 ]
then
	echo $CAPEMGR
	SLOT=$(echo ${CAPEMGR:1:1})
else
	echo "Setting up uart 5 ..."
	sudo sh -c "echo 'BB-UART5' > /sys/devices/platform/bone_capemgr/slots"		
	echo "Done."
fi

# Start data collection 

for i in {1..10}
do
	echo --- Collecting Sample $i ---
	chrt -f 99 ./US.out
	sleep 1
done
