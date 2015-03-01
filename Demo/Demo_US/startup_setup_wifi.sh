#!/bin/bash
#LOCATION=CIC
LOCATION=TEMP
MY_IP=$(ifconfig wlan0 | grep -w "inet" | awk '{print $2}' | sed 's/[A-Za-z:]*//g')
ifconfig | grep wlan0
if [ $? -eq 0 ]
then
	route add default gw 128.237.224.1 
fi
# Update IP
date > last_ip.txt
echo $MY_IP >> last_ip.txt
sshpass -p 'dump1234' ssh dump@sensor.andrew.cmu.edu "mkdir -p ~/data/$LOCATION"
sshpass -p 'dump1234' scp /home/debian/BBBIOlib/Demo/Demo_US/last_ip.txt dump@sensor.andrew.cmu.edu:~/data/$LOCATION/
