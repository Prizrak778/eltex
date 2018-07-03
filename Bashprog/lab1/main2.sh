#!/bin/bash
echo "Введите типовое имя для каталога"
read dir1_name
echo "Введите типовое имя для подкаталога"
read dir2_name
echo "Введите типовое имя для подподкаталога"
read dir3_name
for (( i = 0 ; i < 5; i++ ))
do
	mkdir $dir1_name$i
	for (( j = 0; j < 5; j++))
	do
		mkdir "$dir1_name$i/$dir2_name$j"
		for (( k = 0; k < 5; k++ ))
		do
			mkdir "$dir1_name$i/$dir2_name$j/$dir3_name$k"
		done
	done
done
echo "Ready"

