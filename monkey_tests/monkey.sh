#!/bin/bash

# call from the root folder of minishell ex : ./monkey_tests/monkey.sh

generate_any_string() {
	local chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_``{|}~"
	local length=$(( $RANDOM % 11 + 20 ))
	local result

	for (( i=0; i<$length; i++ )); do
		result="${result:+$result}${chars:$(( $RANDOM % ${#chars} )):1}"
	done
	
	echo "$result"
}


# Test with random ASCII string
for i in {1..10}
do
	ascii_string=$(generate_any_string) 2>monkey_tests/ascii_string.txt
	echo "String: $ascii_string"

	./minishell -c "$ascii_string"
	echo "Exit status minishell: $?"
	./bash -c "$ascii_string" 2>monkey_tests/bash_ascii_string.txt
	echo "Exit status bash: $?"
done

echo ""

# Test with any kind of characters
for i in {1..10}
do
	any_string=$(generate_any_string)
	./minishell -c "$any_string" 2>monkey_tests/any_string.txt

	echo "String test: $any_string"
	echo "Exit status minishell: $?"
	bash -c "$any_string" 2>monkey_tests/bashany_string.txt
	echo "Exit status bash: $?"
done

echo ""

# Test with random command prepended
for i in {1..10}
do
	random_command="cat"
	any_string=$(generate_any_string)
	echo "String: $random_command $any_string"

	./minishell -c "$random_command $any_string" 2>monkey_tests/minishell_random_command.txt
	echo "Exit status minishell: $?"
	bash -c "$random_command $any_string" 2>monkey_tests/random_command.txt
	echo "Exit status bash: $?"
done
