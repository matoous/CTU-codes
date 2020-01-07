#!/bin/bash
# Matous Dzivjak 17. 9. 2017 late in the morning
INPUTFROM=0
if [[ "$#" -eq 0 ]]; then
  echo "Usage:"
    echo "`basename $0` [-i] [-u address]"
    exit 1
elif [[ "$1" == "-h" ]]; then
  echo "Usage:"
    echo "`basename $0` [-i] [-u address]"
    exit 0
elif [[ "$1" == "-i" && "$#" -eq 1 ]]; then
  INPUTFROM="cat"
elif [[ "$1" == "-u" && "$#" -eq 2 ]]; then
  INPUTFROM="wget -O - -q $2"
else
  exit 2
fi

DATA=$($INPUTFROM)
# DELETE FOR DEBUG # >&2 echo "$DATA"
if echo "$DATA" | # pass data in pipe |
    sed ':a;N;$!ba;s/\n/ /g' | # replace newlines with spaces |
    grep -o -i "<a\W+*[^>]*>" | # find all 'a' tags |
    sed -e 's/HREF *= */href=/g' | # replace HREF with href (I mean... who even uses HREF) |
    sed -e 's/href *= */href=/g' | # delete spaces around equal sign |
    sed -e "s/'/\"/g" | # replace single quotes with double quotes |
    sed -n 's/.*href="\([^"]*\).*/\1/p' | # extract links |
    grep -i '\.pdf$'; then # keep only pdf links
  exit 0
else
  exit $?
fi

