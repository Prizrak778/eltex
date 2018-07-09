#!/bin/bash
echo "Выберите действие с будильником для бэкапов"
echo "0 - Создать бэкап; 1 - редактировать бэкап; 2 - удалить бээкап"
read type_get
case $type_get in
	"0")
		echo "Для задания бэкапа нужно ввести некоторые данные"
		echo "Введите минуты(0-59)"
		read min
		echo "Введите часы(0-23)"
		read hour
		echo "Введите день(1-31)"
		read day
		echo "Введите месяц(0-12)"
		read month
		echo "Введите день недели(0-7(0 и 7 = воскресение))"
		read day_week
		echo "0 - блочное копирование"
		read type_get2
		case $type_get2 in
			"0")
				$
		crontab -l > mycron
