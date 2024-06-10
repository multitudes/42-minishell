#!/bin/bash

# call from the root folder of minishell ex : ./log/monkey.sh
# created with help of copilot 🤷🏻‍♂️🤷🏻‍♂️

cd monkey_tests
mkdir -p log

generate_any_string() {
	local chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_``{|}~"
	local length=$(( $RANDOM % 11 + 20 ))
	local result

	for (( i=0; i<$length; i++ )); do
		result="${result:+$result}${chars:$(( $RANDOM % ${#chars} )):1}"
	done
	
	echo "$result"
}

echo "Test with random ASCII string" | tee -a log/ascii_string.txt log/monkeybash.txt  log/monkeyminishell.txt >/dev/null 2>&1
echo | tee -a log/ascii_string.txt log/monkeybash.txt log/monkeyminishell.txt 2>&1

for i in {1..10}
do
	ascii_string=$(generate_any_string) 
	echo "Testing: >>>>>>>>>>>>>>>>>>>>>>>>> " 	$ascii_string | tee -a log/ascii_string.txt log/monkeybash.txt log/monkeyminishell.txt >/dev/null 2>&1

	./minishell -c "$ascii_string" >>log/monkeyminishell.txt 2>&1
	echo "Exit status minishell: $?" >>log/monkeyminishell.txt 2>&1
	echo  >>log/monkeyminishell.txt 2>&1
	bash -c "$ascii_string" >>log/monkeybash.txt 2>&1
	echo "Exit status bash: $?" | tee -a log/monkeybash.txt log/monkeyminishell.txt 2>&1
done

echo "Test with command prepended" | tee -a  log/ascii_string.txt log/monkeybash.txt log/monkeyminishell.txt 2>&1
for i in {1..10}
do
	random_command="cat"
	echo "Command: " $random_command | tee -a log/ascii_string.txt log/monkeybash.txt  log/monkeyminishell.txt 2>&1
	echo |tee -a log/ascii_string.txt log/monkeybash.txt log/monkeyminishell.txt  2>&1
	ascii_string=$(generate_any_string) 
	echo $ascii_string | tee -a log/ascii_string.txt log/monkeyminishell.txt 2>&1

	./minishell -c "$random_command $ascii_string" >>log/monkeyminishell.txt 2>&1
	echo "Exit status minishell: $?" >>log/monkeyminishell.txt 2>&1
	bash -c "$random_command $ascii_string" log/monkeybash.txt 2>&1
	echo "Exit status bash: $?" | tee -a log/monkeybash.txt, log/monkeyminishell.txt 2>&1

done
