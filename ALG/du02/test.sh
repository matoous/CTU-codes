# compile
gcc main.c -o out
# run test
for i in "01" "02" "03" "04" "05" "06" "07" "08" "09" "10"
do
  /usr/bin/time --format='test case '$i' in %e seconds' ./out < datapub/pub$i.in > pub$i.out
  echo "expected: " && cat datapub/pub$i.out && echo "got: " && cat pub$i.out && echo
  rm pub$i.out # delete test output file
done
# clear bin
rm out
