echo "Running unit tests:"

# not a bug below. I do not loop on the c files but on the executables
# therefore even if i dont have valgrind each test executes as a separate compiled program like it had a main in it. this happened already in the makefile but I can run the shell script separately. 
# however I need to execute make first 
for i in *_tests
do
    if test -f $i
    then
        if $VALGRIND ./$i 2>> tests.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's tests.log"
            echo "------"
            tail tests.log
            exit 1
        fi
    fi
done

echo ""
