#!/bin/bash
#//////////////////////////////////////////////////////
#   Created by Iulian Radu, Adrian Salomia
#   Checker Tema 3 - Structuri de date
#   Livrare de produse folosind drone
#   Facultatea de Automatica si Calculatoare
#   Anul Universitar 2016-2017, Seria CC
#/////////////////////////////////////////////////////
result1=0
result2=0
result3=0
result=0
ZERO=0
make build > /dev/null 2>&1
BEST=110
#Task 01
echo " _____________________________________________________________________"
echo "|                               Task 01                               |"
echo "|_____________________________________________________________________|"
for i in {1..5}
do
    fileIn="task1/in/test"$i".in"
    fileOut="task1/out/test"$i".out"
    fileRef="task1/ref/test"$i".ref"
    ./livrare $fileIn $fileOut > /dev/null 2>&1
    diff $fileOut $fileRef > /dev/null
    EXIT_CODE=$?
    rm -rf $fileOut
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Task 1 Test $i ................................................. PASS"
        ((result=result+7))
        ((result1=result1+7))
    else
        echo "Task 1 Test $i ................................................. FAIL"
    fi
done
#Task 2
echo " _____________________________________________________________________"
echo "|                               Task 02                               |"
echo "|_____________________________________________________________________|"
for i in {1..5}
do
    fileIn="task2/in/test"$i".in"
    fileOut="task2/out/test"$i".out"
    fileRef="task2/ref/test"$i".ref"
    ./livrare $fileIn $fileOut > /dev/null 2>&1
    diff $fileOut $fileRef > /dev/null
    EXIT_CODE=$?
    rm -rf $fileOut
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Task 2 Test $i ................................................. PASS"
        ((result=result+6))
        ((result2=result2+6))
    else
        echo "Task 2 Test $i ................................................. FAIL"
    fi
done
#Task 3
echo " _____________________________________________________________________"
echo "|                               Task 3                                |"
echo "|_____________________________________________________________________|"
for i in {1..5}
do
    fileIn="task3/in/test"$i".in"
    fileOut="task3/out/test"$i".out"
    fileRef="task3/ref/test"$i".ref"
    ./livrare $fileIn $fileOut > /dev/null 2>&1
    diff $fileOut $fileRef > /dev/null
    EXIT_CODE=$?
    rm -rf $fileOut
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Task 3 Test $i ................................................. PASS"
        ((result=result+5))
        ((result3=result3+5))
    else
        echo "Task 3 Test $i ................................................. FAIL"
    fi
done
#Bonus
bonus=0
echo " _____________________________________________________________________"
echo "|                                Bonus                                |"
echo "|_____________________________________________________________________|"
for i in {1..5}
do
    fileIn="bonus/in/test"$i".in"
    fileOut="bonus/out/test"$i".out"
    fileRef="bonus/ref/test"$i".ref"
    ./livrare $fileIn $fileOut > /dev/null 2>&1
    diff $fileOut $fileRef > /dev/null
    EXIT_CODE=$?
    rm -rf $fileOut
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Bonus Test $i .................................................. PASS"
        ((result=result+4))
        ((bonus=bonus+4))
    else
        echo "Bonus Test $i .................................................. FAIL"
    fi
done
echo "Total - Task 1: "$result1
echo "Total - Task 2: "$result2
echo "Total - Task 3: "$result3
echo "Total - Bonus:  "$bonus
if [ "$result" -eq $BEST ]; then
    echo "Felicitari! Ai punctajul maxim: "$BEST"p! :)"
else
    echo "Ai acumulat "$result"p din maxim 110p! :("
fi
make clean > /dev/null 2>&1
