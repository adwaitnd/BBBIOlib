#!/bin/bash

CAPEMGR=$(cat /sys/devices/platform/bone_capemgr/slots | grep UART5)

#remove 
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
