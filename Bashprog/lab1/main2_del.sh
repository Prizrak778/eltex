#!/bin/bash
echo "Введите имя типового каталога"
read dir1_name
for (( i=0; i<5; i++ ))
do
	rm -rf $dir1_name$i
done 
echo "Ready"
