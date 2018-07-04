#!/bin/bash
if [[ $# -eq 0 ]]
then
	pwd_script=`pwd`
	script_name="$pwd_script/$0"
	echo "==============================================="
	echo "0 - создание контроль процесса, 1 - удалить контроль процесса, 2 - редактирование контроля процессора"
	read type_get
	case $type_get in
		"0")
			echo "Для задания контроля процесса нужно ввести некоторое данные"
			echo "Укажите какой процесс будет контролироваться"
			read exe_name
			echo "Введите переодичность проверки в минутах(каждые 2 минуты ,указать 2)"
			read min
			crontab -l>mycron
			echo "$min * * * * DISPLAY=:0.0 $script_name $exe_name control_process">>mycron
			crontab mycron
			rm mycron
		;;
		"1")
			echo "0 - удалить все, 1 - удалить определённый"
			read type2_get
			case $type2_get in
				"0")
					crontab -l|grep -v "control_process">mycron
					crontab mycron
					rm mycron
					;;
				"1")
					echo "Список контролей:"
					crontab -l|grep "control_process">mycron
					iter=1
					while read LINE
					do
						echo "№$iter: $LINE"
						let iter=$iter+1
					done<mycron
					echo "Для удаления укажите номер контроля"
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
						crontab -l| grep -v "$line_cron">mycron
						crontab mycron
						rm mycron
					else
						echo "Контроля с таким номером нет"
					fi
					;;
			esac
		;;
		"2")
			echo "Список контролей:"
			crontab -l|grep "control_process">mycron
			iter=1
			while read LINE
			do
				echo "№$iter: $LINE"
				let iter=$iter+1
			done<mycron
			echo "Для редактирования укажите номер контроля"
			read num_cron
			if [[ $num_cron < $iter ]]
			then
				echo "Ddtlbnt"
				iter=1
				while read LINE
				do
					if [[ $iter = $num_cron ]]
					then
						line_cron=$LINE
					fi
					let iter=$iter+1
				done<mycron
				crontab -l| grep -v "$line_cron">mycron
				crontab mycron
				rm mycron
			else
				echo "Контроля с таким номером нет"
			fi 
		;;
	esac
else
	
fi
