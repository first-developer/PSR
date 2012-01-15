#!/bin/bash


for i in `ipcs -q | grep 0x | cut -f2 -d\ `
do 
  ipcrm -q $i
done


