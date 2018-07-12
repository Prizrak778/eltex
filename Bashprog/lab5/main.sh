#!/bin/bash 
DIALOG=${DIALOG=dialog}
$DIALOG --title "Текстовый редактор" --clear \
		--yesno "Открыть файл" 10 40
case $? in
	0)
		FILE_GET=`$DIALOG --stdout --title "Выберите файл" --fselect $HOME/ 10 60` 
		case $? in 
			0) 
				TEXT_CHANGE=`$DIALOG --stdout --title "Редактирование" --editbox $FILE_GET 40 100`
				case $? in
					0)
						clear 
						echo "Изменения созранены" 
						echo $TEXT_CHANGE>$FILE_GET
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
		clear
		echo "Ну и пошёл ты." 
		;; 
	255) 
		clear 
		echo "Выход" 
		;; 
esac 
