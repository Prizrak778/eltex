#!/bin/bash
if [[ $# -eq 0 ]]
then
	pwd_dir=`pwd`
	script_name="$pwd_dir/$0"
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
			echo "Введите что вы хотите забэкапить(полный путь)"
			read dir_save
			if test -b $type_file
			then
				type_get2=0
				type_backup="dd"
			elif test -d $type_file
			then
				type_get2=2
				type_backup="rsync"
			elif test -f $type_file
			then
				type_backup="tar"
				echo "Заархивировать файл? 0 - да, 1 - нет"
				read type_get2
				if [[ $type_get2 = "0" ]]
				then
					type_get2=1
				else
					type_get2=3
				fi
			else
				echo "Невозможно копировать данный тип"
				exit 0
			fi
			echo "Введите где вы хотите сохранить(полный путь)"
			read dir_backup
			echo "$min$hour$day$month$day_week$dir_save$dir_backup$type_backup">>.config.txt
			echo "$dir_save">>.config.txt
			echo "$dir_backup">>.config.txt
			echo "$type_get2">>.config.txt
			crontab -l > mycron
			echo "$min $hour $day $month $day_week DISPLAY=:0.0 $script_name<$pwd_dir/.config.txt $min$hour$day$month$day_week$dir_save$dir_backup$type_backup backup">>mycron
			crontab mycron
			rm mycron
			;;
		"1")
			echo "Список будильников"
			crontab -l > mycron
			iter=1
			while read LINE
			do
				echo "№$iter: $LINE"
				let iter=$iter+1
			done < mycron| grep "backup"
			echo "Для редактирования бэкапа укажите номер бэкапа"
			read num_cron
			if [[ $num_cron<$iter ]]
			then
				iter=1
				while read LINE
				do
					if [[ $iter = $num_cron ]]
					then 
						line_cron=$LINE
					fi
					let iter=$iter+1
				done<mycron
				crontab -l|grep -v "$line_cron">mycron
				while read LINE
				do
					if [[ $iter = $num_cron ]]
					then
						line_cron=$LINE
					fi
					let iter=$iter+1
				done<.config.txt
				cat .config.txt|grep -v $LINE -A 3>.config.txt
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
				echo "Введите что вы хотите забэкапить(полный путь)"
				read dir_save
				echo "Введите где вы хотите сохранить(полный путь)"
				read dir_backup
				type_file=`ls -all $dir_save`
				type_file=`echo ${type_file:0:1}`
				if [[ $type_file -eq b ]]
				then
					type_get2=0
					type_backup="dd"
				elif [[ $type_file -eq d ]]
				then
					type_get2=2
					type_backup="rsync"
				else
					type_backup="tar"
					echo "Заархивировать файл? 0 - да, 1 - нет"
					read type_get2
					if [[ $type_get2 -eq 0 ]]
					then
						type_get2=1
					else
						type_get2=3
					fi
				fi
				echo "$min$hour$day$month$day_week$dir_save$dir_backup$type_backup">>.config.txt
				echo "$dir_save">>.config.txt
				echo "$dir_backup">>.config.txt
				echo "$type_get2">>.config.txt
				echo "$min $hour $day $month $day_week DISPLAY=:0.0 $script_name<$pwd_dir/.config.txt $min$hour$day$month$day_week$dir_save$dir_backup$type_backup backup">>mycron
				crontab mycron
				rm mycron
			else
				echo "Бэкапа с таким номером нет"
			fi
			;;
		"2")
			echo "0 - удалить все, 1 - уалить определённые будильник"
			read type_del
			case $type_del in
				"0")
					crontab -r;
					rm .config.txt
					;;
				"1")
					echo "Списко бэкапов:"
					crontab -l > mycron
					iter=1
					while read LINE
					do
						echo "№$iter: $LINE"
						let iter=$iter+1
					done < mycron| grep "backup"
					echo "Для удаления укажите номер будильника"
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
					done<mycron
					crontab -l|grep -v "$line_cron">mycron
					while read LINE
					do
						if [[ $iter = $num_cron ]]
						then
							line_cron=$LINE
						fi
						let iter=$iter+1
					done<.config.txt
					cat .config.txt|grep -v $LINE -A 3>.config.txt
					crontab mycron
					rm mycron
					else
						echo "Бэкап с таким номером отсутствует"
					fi
					;;
			esac
			;;
	esac
else
	flag=1
	while [ $flag -eq 1 ]
	do
		read key
		if [[ "$key" == $1 ]]
		then
			flag=0
		fi
	done
	read dir_save
	read dir_backup
	read type_backup
	case "$type_backup" in
			"0")
				dd if=$dir_save of=$dir_backup bs=4096
				;;
			"1")
				/bin/tar  -cvzf "$dir_backup.tar" "$dir_save"
				;;
			"2")
				rsync -avz "$dir_save" "$dir_backup"
				;;
			"3")
				/bin/tar -cvf "$dir_backup.tar" "$dir_save"
				;;
	esac
fi
exit 0

