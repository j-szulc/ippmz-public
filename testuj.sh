#!/bin/bash
prog=$1
dir=$2
valgrind_flags="--quiet --leak-check=full --error-exitcode=15 \
                --show-leak-kinds=all --errors-for-leak-kinds=all \
                --log-file=/dev/null"

stdout_result=$(mktemp)
stderr_result=$(mktemp)
# Remove temporary files on exit
trap "rm -f $stdout_result $stderr_result" EXIT

for inputfile in $2/*.in; do

  [[ -e "$inputfile" ]] || continue
  outputfile="${inputfile%.in}.out"
  errorfile="${inputfile%.in}.err"

  echo "Processing: $inputfile"

  valgrind $valgrind_flags $prog $inputfile >$stdout_result 2>$stderr_result
  if [[ "$?" -eq 0 ]]; then
    echo "VALGRIND: OK"
  else
    echo "VALGRIND: FAILED"
  fi

  cmp --silent $stdout_result $outputfile
  stdout_cmp=$?

  cmp --silent $stderr_result $errorfile
  stderr_cmp=$?

  if [[ "$stdout_cmp" -eq 0 ]] && [[ "$stderr_cmp" -eq 0 ]]; then
    echo "RESULT: OK"
  else
    echo "RESULT: FAILED"
  fi

done
