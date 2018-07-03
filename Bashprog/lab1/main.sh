#!/bin/bash
echo "Введите первое число"
read num1
echo "Введите второе число"
read num2
echo "Выбирете операцию, которую хотите сделать с двумя числами"
read oper
awk 'BEGIN{print ('"$num1"''"$oper"''"$num2"')}'
if [[ $oper = '/' ]]
then
	let num3=$num1$oper$num2
	echo  -n $num3.
	for (( i = 0; i < 32 && $num1%$num2!=0; i++ ))
	do
		if [[ $(($num1%$num2)) != 0 ]]
		then
			let num1=($num1-$num1/$num2*$num2)*10
		fi
		let num3=$num1$oper$num2
		echo -n $num3
	done
	echo " "
else
	let num1=$num1$oper$num2
	echo $num1
fi

