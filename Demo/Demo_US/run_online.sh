#!/bin/bash

PERIOD=60
#DELAY=2
#LOCATION=SCAIF

while true
do

  # Check Wifi setup
  #./setup_wifi.sh $LOCATION

  # Change cpu governor
  echo "performance" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 

  # Add UART overlay
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
  fi
  # Start data collection 
  chrt -f 99 ./US_online.out 100
  # Take some rest
  echo "Wait for next round in $PERIOD(s) ..."
  sleep $PERIOD

done
