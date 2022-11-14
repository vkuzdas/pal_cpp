for i in 01 02 03 04 05 06 07 08 09 10 ; do
	time ./a.out < ./datapub/pub${i}.in > out
  if ! diff ./datapub/pub${i}.out out ; then
    echo "CHYBA pub${i}.in
    "
#    exit 1
  fi
done
