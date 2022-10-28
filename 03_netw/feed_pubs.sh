for i in 10 ; do
	./a.out < ./datapub/pub${i}.in > out
  if ! diff ./datapub/pub${i}.out out ; then
    echo "CHYBA pub${i}.in
    "
#    exit 1
  fi
done
