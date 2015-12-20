#!/bin/bash

ifconfig | grep wlan0
if [ $? -eq 1 ]
then
	ifup wlan0
	sleep 15	
fi

RES=$(ping 8.8.8.8 -c 1 | egrep 'Unreachable | unreachable' | wc -l)
while [ $RES -ne 0 ]
do
	ifdown wlan0
	sleep 3
	ifup wlan0
	sleep 15
	RES=$(ping 8.8.8.8 -c 1 | egrep 'Unreachable | unreachable' | wc -l)
done

# Update time 
echo "Syncing time ... "
ntpdate -b -u pool.ntp.org
# Update IP
echo "Update IP ..."
date > last_ip.txt
ifconfig wlan0 | grep inet >> last_ip.txt
 
