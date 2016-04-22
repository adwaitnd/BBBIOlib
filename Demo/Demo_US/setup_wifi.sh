#!/bin/bash
#LOCATION=CIC
LOCATION=$1

route -n | grep 128.237.224.1
if [ $? -eq 1 ]
then
	echo "Reseting IP table ..."
	#route del default gw 192.168.1.1
	route add default gw 128.237.224.1 
	#ifdown wlan0
	#sleep 3
	#ifup wlan0
	#sleep 15
fi

MY_IP=$(ifconfig wlan0 | grep -w "inet" | awk '{print $2}' | sed 's/[A-Za-z:]*//g')

ifconfig | grep wlan0
if [ $? -eq 1 ]
then
	ifup wlan0
	sleep 15	
	route add default gw 128.237.224.1 
fi

RES=$(ping 8.8.8.8 -I wlan0 -c 1 | egrep 'Unreachable|unreachable' | wc -l)
sleep 3
echo $RES

while [ $RES -ne 0 ]
do
	echo "Ping failed, reseting the interface ..."
	ifdown wlan0
	sleep 3
	ifup wlan0
	sleep 15
	route add default gw 128.237.224.1 
	RES=$(ping 8.8.8.8 -c 1 | egrep 'Unreachable|unreachable' | wc -l)
done

# Update time 
echo "Syncing time ... "
ntpdate -b -u pool.ntp.org
# Update IP
echo "Updating IP ..."
date > last_ip.txt
echo $MY_IP >> last_ip.txt
echo "Create folder $LOCATION"
sshpass -p 'dump1234' ssh dump@sensor.andrew.cmu.edu "mkdir -p ~/data/$LOCATION"
sshpass -p 'dump1234' scp /home/debian/BBBIOlib/Demo/Demo_US/last_ip.txt dump@sensor.andrew.cmu.edu:~/data/$LOCATION/
echo "Wifi update done."
