#!/bin/bash
# call from the root folder of minishell ex : ./monkey_tests/monkey.sh
# created with help of copilot 🤷🏻‍♂️🤷🏻‍♂️
generate_any_string() {
    local chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_``{|}~"
    local length=$(( $RANDOM % 11 + 20 ))
    local result
    for (( i=0; i<$length; i++ )); do
        result="${result:+$result}${chars:$(( $RANDOM % ${#chars} )):1}"
    done
    
    echo "$result"
}

echo "Test with random ASCII string" | tee -a monkey_tests/ascii_string.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1
echo  | tee -a monkey_tests/ascii_string.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1

for i in {1..10}
do
    ascii_string=$(generate_any_string) 
    echo -e "Testing: >>>>>>>>>>>>>>>>>>>>>>>>>\n $ascii_string" | tee -a monkey_tests/ascii_string.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1

    ./minishell -c "$ascii_string" >>monkey_tests/monkeyminishell.txt 2>&1
    echo "Exit status minishell: $?" >>monkey_tests/monkeyminishell.txt 2>&1
    echo | tee -a monkey_tests/monkeyminishell.txt >/dev/null 2>&1
    ./bash -c "$ascii_string" >>monkey_tests/monkeybash.txt 2>&1
    echo "Exit status bash: $?" | tee -a monkey_tests/monkeybash.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1
done

echo "Test with command prepended" | tee -a monkey_tests/monkeybash.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1

for i in {1..10}
do
    random_command="cat"
    echo $random_command >>monkey_tests/monkeyminishell.txt 2>&1
    echo -e "\n" >>monkey_tests/monkeyminishell.txt 2>&1
    random_command=$(generate_any_string)
    echo $ascii_string | tee -a monkey_tests/ascii_string.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1
    ./minishell -c "$random_command $any_string" >>monkey_tests/monkeyminishell.txt 2>&1
    echo "Exit status minishell: $?" >>monkey_tests/monkeyminishell.txt 2>&1
    ./bash -c "$random_command $any_string" | tee -a monkey_tests/monkeybash.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1
    echo "Exit status bash: $?" | tee -a monkey_tests/monkeybash.txt monkey_tests/monkeyminishell.txt >/dev/null 2>&1
done