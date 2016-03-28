#! /bin/bash
### BEGIN INIT INFO
# Provides:          auto_ip
# Required-Start:    $all
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:
# Short-Description: Run auto_ip after bootup
### END INIT INFO

TLOC=IP_log

do_start() {
	sleep 60
	ntpdate -b -u pool.ntp.org
	MY_IP=$(ifconfig wlan0 | grep -w "inet" | awk '{print $2}' | sed 's/[A-Za-z:]*//g')
	date > /home/debian/BBBIOlib/Demo/Demo_US/last_ip.txt
 	echo $MY_IP >> /home/debian/BBBIOlib/Demo/Demo_US/last_ip.txt
 	sshpass -p 'dump1234' ssh dump@sensor.andrew.cmu.edu "mkdir -p ~/data/$TLOC"
	sshpass -p 'dump1234' ssh dump@sensor.andrew.cmu.edu "cat >> ~/data/$TLOC/last_ip.txt" < /home/debian/BBBIOlib/Demo/Demo_US/last_ip.txt
}

case "$1" in
    start)
	do_start
        ;;
    restart|reload|force-reload)
        echo "Error: argument '$1' not supported" >&2
        exit 3
        ;;
    stop)
        ;;
    *)
        echo "Usage: $0 start|stop" >&2
        exit 3
        ;;
esac
