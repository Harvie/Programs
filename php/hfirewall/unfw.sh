#!/bin/sh
#HFirewall Uninstall 0.1 Alpha
#This will disable IPTables blocking

iptables --flush
iptables --policy INPUT ACCEPT
iptables --policy OUTPUT ACCEPT
iptables --policy FORWARD ACCEPT

