#!/bin/sh
sudo rmmod farm_mod
sudo insmod farm_mod.ko
sudo rm -rf /dev/farm_dev
sudo sh mknod.sh
echo "-----------------------"
