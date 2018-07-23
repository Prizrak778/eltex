#!/bin/bash 
DIALOG=${DIALOG=dialog}
$DIALOG --title "Текстовый редактор" --clear \
		--yesno "Открыть файл" 10 40
case $? in
	0)
		FILE_GET="`$DIALOG --stdout --title "Выберите файл" --fselect $HOME/ 10 60`" 
		case $? in 
			0) 
				TEXT_CHANGE="`$DIALOG --stdout --title "Редактирование" --editbox $FILE_GET 40 100`"
				case $? in
					0)
						clear 
						echo "Изменения сохранены" 
						echo "$TEXT_CHANGE">$FILE_GET
						exit 0 
						;; 
					1) 
						clear 
						echo "Изменения не сохранены" 
						exit 0 
						;; 
					255) 
						clear
						 echo "Выход" 
						;; 
				esac 
				;; 
			1) 
				clear 
				echo "Отказ от ввода. " 
				exit 0 
				;; 
			255) 
				clear 
				echo "Нажата клавиша ESC." 
				exit 0 
				;; 
		esac 
		;; 
	1) 
		FILE_GET="`$DIALOG --stdout --title "Введите название файла" --inputbox "Поле ввода" 16 51`"
		
		case $? in 
			0)
				DIR_SELECT="`$DIALOG --stdout --title "Введите путь куда сохранить файл" --dselect $HOME/ 10 40`"
				case $? in
				0)
					FILE_NEW="$DIR_SELECT$FILE_GET"
					touch "$FILE_NEW"
					echo "$FILE_NEW"
					TEXT_CHANGE="`$DIALOG --stdout --title "Редактирование" --editbox "$FILE_NEW" 40 100`"
					case $? in
						0)
							clear 
							echo "Изменения сохранены" 
							echo "$TEXT_CHANGE">"$FILE_NEW"
							exit 0 
							;; 
						1) 
							clear 
							echo "Изменения не сохранены" 
							exit 0 
							;; 
						255) 
							clear
							echo "Выход" 
							;; 
					esac 
				;;
				1)
					clear
					echo "Не было определено место хранения файла"
					exit 0
				;;
				255)
					clear
					echo "Нажата клавиша ESC." 
					exit 0 
				;;
				esac
			;;
			1) 
				clear 
				echo "Отказ от ввода." 
				exit 0 
			;; 
			255) 
				clear 
				echo "Нажата клавиша ESC." 
				exit 0 
			;; 
			esac 
		clear
		;; 
	255) 
		clear 
		echo "Выход" 
	;; 
esac 

