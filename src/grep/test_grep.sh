#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
VG_RES=""

declare -a multy_testing=(
"D VAR test1.txt"
"S VAR test1.txt test2.txt test1.txt"
"-e s VAR test3.txt"
"-e ^int VAR test1.txt test2.txt test1.txt"
"VAR -f pattern_file.txt test3.txt test2.txt test1.txt"
# "-f pattern_file.txt VAR test3.txt test2.txt test6.txt"
)

declare -a unique_testing=(
"abc no_file.txt"
"abc -f no_file -ivclnhso no_file.txt"
"-e S -i -nh test2.txt test1.txt test2.txt test1.txt"
"-e char -v test2.txt test1.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
        echo "TEST $COUNTER: OK"
        echo "grep $t"
    else
      (( FAIL++ ))
        echo "TEST $COUNTER: FAIL!!!!!!!!!!!!!!!!!!!!! $FAIL"
        echo "grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

for var1 in i v c l n h s
do
  for i in "${multy_testing[@]}"
  do
    var="-$var1"
    testing $i
  done
done

for var1 in i v c l n h s
do
  for var2 in i v c l n h s
  do
    for i in "${multy_testing[@]}"
      do
        var="-$var1$var2"
        testing $i
     done
  done
done

for var1 in i v c l n h s
do
  for var2 in i v c l n h s
  do
    for var3 in i v c l n h s
    do
      for i in "${multy_testing[@]}"
      do
        var="-$var1$var2$var3"
        testing $i
      done   
    done
  done
done

for var1 in i v c l n h s o
do
 for var2 in i v c l n h s o
 do
   for var3 in i v c l n h s o
   do
     for i in "${o_mac_testing[@]}"
     do
       var="-$var1$var2$var3"
       testing $i
     done   
   done
 done
done

for i in "${unique_testing[@]}"
do
    var="-"
    testing $i
done

# # tests for -o flag, relised in linux style"
# for var1 in i v c l n h s o 
# do
#   for var2 in i v c l n h s o
#   do
#     for i in "${multy_testing[@]}"
#       do
#         var="-$var1$var2"
#         testing $i
#       done
#   done
# done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
