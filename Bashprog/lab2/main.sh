#!/bin/bash
echo "Выберите действие с будильником"
echo "0 - создать будильник; 1 - редактировать; 2 - удалить;"
read type_get
case $type_get in
	"0")
		echo "Для задания будильника нужно ввести некоторые данные"
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
		crontab -l > mycron
		echo "$min $hour $day $month $day_week DISPLAY=:0.0 /usr/bin/thunar">>mycron
		crontab mycron
		rm mycron
		;;
	"1")
		echo "Список будтльников:"
		crontab -l > mycrontab
		iter=1
		while read LINE;
		do 
			echo "№$iter: $LINE"
			let iter=$iter+1
		done < mycrontab
		echo "Для редактирования укажите номер будильника, который вы хотите изменить"
		read num_cron
		if [[ $num_cron < $iter ]]
		then
			iter=1
			while read LINE
			do
				if [[ $iter = $num_cron ]]
				then
					line_cron=$LINE
				fi
				let iter=$iter+1
			done < mycrontab
			crontab -l| grep -v "$line_cron">mycron
			echo "Введите исправления"
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
			echo "$min $hour $day $month $day_week DISPLAY=:0.0 /usr/bin/thunar">>mycron
			crontab mycron
			rm mycron
		else
			echo "Будильник с таким номером отсутствует"
		fi
		;;
	"2")
		echo "0 - удалить все, 1 - удалить определённый будильник"
		read type_del
		case $type_del in
			"0")
				crontab -r
				;;
			"1")
				echo "Список будтльников:"
				crontab -l > mycrontab
				iter=1
				while read LINE;
				do 
					echo "№$iter: $LINE"
					let iter=$iter+1
				done < mycrontab
				echo "Для редактирования укажите номер будильника, который вы хотите изменить"
				read num_cron
				if [[ $num_cron < $iter ]]
				then
				iter=1
				while read LINE
				do
					if [[ $iter = $num_cron ]]
					then
						line_cron=$LINE
					fi
					let iter=$iter+1
				done < mycrontab
				crontab -l| grep -v "$line_cron">mycron
				crontab mycron
				rm mycron
				else
					echo "Будильника с таким номером нет"
				fi
				;;
		esac
		;;
esac
exit 0

