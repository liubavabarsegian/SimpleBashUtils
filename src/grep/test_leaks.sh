leakmacro () {
  leaks -atExit -- ./s21_grep $1 $2 $3 $4 $5 $6 $7 $8 $9 > leak.log
  grep -i errors leak.log 
}

filename1=test_leaks.sh;
filename2=grep.c;
filename3=pattern_file.txt
filename4=grep.h;
pattern1=S

echo ----------------------------------------------*GREP LEAK CHECK 1
leakmacro ./s21_grep "$pattern1" "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 1 -e
leakmacro ./s21_grep -e "$pattern1" "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 2
leakmacro ./s21_grep -i "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 3
leakmacro ./s21_grep -v "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 4
leakmacro ./s21_grep -c "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 5
leakmacro ./s21_grep -l "$pattern1" "$filename2" "$filename3" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 6
leakmacro ./s21_grep -n "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 7 BONUS
leakmacro ./s21_grep -h "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 8 BONUS
leakmacro ./s21_grep -o "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 9 BONUS
leakmacro ./s21_grep -h "$pattern1" "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 10 BONUS
leakmacro ./s21_grep "$pattern1" -s aboba
echo ----------------------------------------------*GREP LEAK CHECK 11 BONUS
leakmacro ./s21_grep -f "$filename3" "$filename2"
echo ----------------------------------------------*GREP LEAK CHECK 12 BONUS
leakmacro ./s21_grep -in "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 13 BONUS
leakmacro ./s21_grep -cv "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 14 BONUS
leakmacro ./s21_grep -iv "$pattern1" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 15 BONUS
leakmacro ./s21_grep -lv "$pattern1" "$filename2" "$filename3" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 16 BONUS
leakmacro ./s21_grep -ho "$pattern1" "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 17 BONUS
leakmacro ./s21_grep -nf "$filename3" "$filename2"
echo ----------------------------------------------*Leak tests ended*
