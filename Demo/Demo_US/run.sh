#!/bin/bash

PERIOD=600
DELAY=2

while true
do

  # Check Wifi setup
  ./setup_wifi.sh

  # Update Time
  echo "Syncing time ... "
  ntpdate -b -u pool.ntp.org

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
  for i in {1..5}
  do
	echo --- Collecting Sample $i ---
	chrt -f 99 ./US.out
	DATE=$(date +"%Y-%m-%d_%H:%M:%S")
	streamer -f jpeg -o $DATE.jpeg	
	sleep $DELAY
  done

  DAY=$(date +"%Y-%m-%d")
  #dump to server
  sshpass -p 'dump1234' ssh dump@sensor.andrew.cmu.edu "mkdir -p ~/data/$DAY/"
  sshpass -p 'dump1234' scp /home/debian/BBBIOlib/Demo/Demo_US/*.{dat,jpeg} dump@sensor.andrew.cmu.edu:~/data/$DAY/
  if [ $? -eq 0 ]
  then
    echo "Data dump complete, cleaning up ..."
    rm /home/debian/BBBIOlib/Demo/Demo_US/*.{dat,jpeg}
  fi

  # Take some rest
  echo "Wait for next round in $PERIOD(s) ..."
  sleep $PERIOD

done
