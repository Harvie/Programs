#!/bin/bash

#BINARIES
screen='/usr/bin/screen';

#COMMON
cd /mnt/data2/uml/;                     #directory to work in
umluser='vserver';                      #Setuid before run vserver
umldelay='10';                          #Delay (man sleep) before restart after vserver halt/fail
umldir='/mnt/data2/uml/umldir';         #Directory for umid & pid files

#NETWORK
hostnic='eth1';                         #network interface to operate on
umlbridge='br0';                        #name of bridge between real network and UMLs
hostip='192.168.2.163';                 #ipadres of host
hostmask='255.255.255.0';               #netmask of host
hostgw='192.168.2.1';                   #default gw of host

#MACHINES
uml_machines() {                        #list of UML machines to be executed
        #new_uml        respawn umlkernel                       umlrootimg      umlmem  umlnice umllog          tapnic  umlmac
        #new_uml        true    linux.uml                       debtest.img     64M     10      /dev/null       tap0    fe:fd:c0:a8:02:89
        new_uml         true    /opt/uml/linux-2.6.24-rc7       debtest.img     64M     10      log0.txt        tap0    fe:fd:c0:a8:02:89
}

############################################################
echo "

!!!!!!YOU MAY NEED TO SETUP YOUR GUEST OS!!!!!!!
!!!!!!!!!!OR YOU CAN USE DHCP INSTEAD!!!!!!!!!!!
root@uml# cat /etc/network/interfaces

auto lo
iface lo inet loopback

auto eth0
iface eth0 inet static
address YOUR.ADD.RESS.HERE
netmask $hostmask
gateway $hostgw
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

";
############################################################

echo [i] Initializing tuntap networking...
modprobe tun

echo [i] Initializing "$umlbridge" bridge...
ifconfig "$hostnic" 0.0.0.0 promisc up
brctl addbr "$umlbridge"
brctl setfd "$umlbridge" 0
brctl sethello "$umlbridge" 0
brctl stp "$umlbridge" off

echo [i] Adding "$hostnic" interface to "$umlbridge" bridge...
ifconfig "$umlbridge" "$hostip" netmask "$hostmask" up
route add default gw "$hostgw"
brctl addif "$umlbridge" "$hostnic"

echo [!] Now launching all UML Machines...
new_uml() {
        if [ -z "$8" ]; then
                echo [!] Failed. Not enough parameters for this machine.;
                return;
        fi;

        respawn="$1";
        umlkernel="$2";
        umlrootimg="$3";
        umlmem="$4";
        umlnice="$5";
        umllog="$6";
        tapnic="$7";
        umlmac="$8";

        tunctl -d "$tapnic"
        tunctl -u "$umluser" -t "$tapnic"

        echo [i] Adding "$tapnic" interface to "$umlbridge" bridge...
        ifconfig "$tapnic" 0.0.0.0 promisc up
        brctl addif "$umlbridge" "$tapnic"

        #echo [R] Sleeping 120 before REBOOT.... #DEBUG FEATURE
        #sleep 120;
        #reboot;


        echo [S] Starting UML on "$tapnic"...

        #con0=fd:0,fd:1 con1=pty
        #$screen -d -m su "$umluser" -c "nice -n $umlnice $umlkernel mem=$umlmem ubd0=$umlrootimg jail eth0=tuntap,$tapnic,$umlmac uml_dir=$umldir umid=$tapnic con0=fd:0,fd:1 con1=pty > $umllog 2>&1"
        #su "$umluser" -c "$screen -d -m nice -n $umlnice $umlkernel mem=$umlmem ubd0=$umlrootimg jail eth0=tuntap,$tapnic,$umlmac uml_dir=$umldir umid=$tapnic con0=fd:0,fd:1 con1=pty > $umllog 2>&1"
        #nice -n "$umlnice" su "$umluser" -c "$screen -d -m $umlkernel mem=$umlmem ubd0=$umlrootimg jail eth0=tuntap,$tapnic,$umlmac uml_dir=$umldir umid=$tapnic con0=fd:0,fd:1 con1=pty > $umllog 2>&1"

        nice -n "$umlnice" su "$umluser" -c "$screen -d -m bash -c '
                while true; do
                        $umlkernel mem=$umlmem ubd0=$umlrootimg jail eth0=tuntap,$tapnic,$umlmac uml_dir=$umldir umid=$tapnic con0=fd:0,fd:1 con1=pty > $umllog 2>&1;
                        sleep $umldelay;
                        if ( $respawn ); then continue; fi; break;
                done;
        '";
}

uml_machines;
echo [!] All UML Machines launched.
