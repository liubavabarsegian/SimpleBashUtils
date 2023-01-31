leakmacro () {
  leaks -atExit -- ./s21_cat $1 $2 $3 $4 $5 $6 $7 $8 $9 > leak.log
  grep -i errors leak.log 
}

filename=Makefile;
echo ----------------------------------------------*-b
leakmacro ./s21_cat -b "$filename"
echo ----------------------------------------------*-e
leakmacro ./s21_cat -e "$filename"
echo ----------------------------------------------*-n
leakmacro ./s21_cat -n "$filename"
echo ----------------------------------------------*-n
leakmacro ./s21_cat "$filename" -n
echo ----------------------------------------------*-s
leakmacro ./s21_cat -s "$filename"
echo ----------------------------------------------*-t
leakmacro ./s21_cat -t "$filename"
echo ----------------------------------------------*-E
leakmacro ./s21_cat -E "$filename"
echo ----------------------------------------------*-bne
leakmacro ./s21_cat -bne "$filename"
echo ----------------------------------------------*-benst
leakmacro ./s21_cat -benst "$filename"
echo ----------------------------------------------*nothing
leakmacro ./s21_cat "$filename"
echo ----------------------------------------------*Leak tests ended*