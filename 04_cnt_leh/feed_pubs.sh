for i in 01 02 03 04 05 06 07 08 09 10 ; do
	echo "pub${i}.in computed in:"
	time ./a.out < ./datapub/pub${i}.in > out
	echo "------"
  if ! diff ./datapub/pub${i}.out out ; then
    echo "CHYBA pub${i}.in
    "
#    exit 1
  fi
done
