#!/bin/bash

PERIOD=600
DELAY=2
LOCATION=SCAIF

while true
do

  # Check Wifi setup
  ./setup_wifi.sh $LOCATION

  # Update Timei in wifi setup
  #echo "Syncing time ... "
  #ntpdate -b -u pool.ntp.org

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

  ## Warm up the transformer
  chrt -f 90 ./US.out 0
  sleep $DELAY

  for i in {1..5}
  do
	echo --- Collecting Sample $i ---
	VOL=$((i*10+50))
	chrt -f 99 ./US.out $VOL
	sleep 1
	chrt -f 99 ./US.out $VOL
	sleep 1
	chrt -f 99 ./US.out $VOL
	sleep 1
	chrt -f 99 ./Tone.out
	sleep 0.5
	DATE=$(date +"%Y-%m-%d_%H:%M:%S")
	streamer -f jpeg -o $DATE.jpeg	
	sleep $DELAY
  done

  DAY=$(date +"%Y-%m-%d")
  #dump to server
  sshpass -p 'trash5566' ssh dump@sensor.andrew.cmu.edu "mkdir -p ~/data/$LOCATION/$DAY/"
  sshpass -p 'trash5566' scp /home/debian/BBBIOlib/Demo/Demo_US/*.{dat,jpeg,raw} dump@sensor.andrew.cmu.edu:~/data/$LOCATION/$DAY/
  if [ $? -eq 0 ]
  then
    echo "Data dump complete, cleaning up ..."
    rm /home/debian/BBBIOlib/Demo/Demo_US/*.{dat,jpeg,raw}
  fi

  # Take some rest
  echo "Wait for next round in $PERIOD(s) ..."
  sleep $PERIOD

done
