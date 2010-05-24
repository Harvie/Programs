#!/bin/bash

echo ModProbing modules...
  ifconfig ath0 down
  ifconfig wifi0 down
  modprobe ath_pci
  modprobe wlan_scan_sta

echo Recreating ath0 device from wifi0...
  ifconfig wifi0 up
  wlanconfig ath0 destroy
  wlanconfig ath0 create wlandev wifi0 wlanmode sta
  ifconfig ath0 up

echo Scanning for APs...
  sleep 3
  wlanconfig ath0 list scan

echo Using strongest AP...
  iwconfig ath0 essid any
  ifconfig essid Network_395 ath0

echo DHCP Broadcast...
  dhclient ath0

echo Done!
