#!/usr/bin/env bash

PROG_PATH=${1}

VALGRIND_LOG="valgrind.log"
NO_LOST_PATTERN="All heap blocks were freed -- no leaks are possible"
NO_ERROR_PATTERN="ERROR SUMMARY: 0 errors"

if [ -z "$1" ]; then
  echo "Usage: run ./valgrind/run.sh /path/to/bin"
  exit 1
fi

RECEIVED=$(echo "${IN}" | eval "valgrind --tool=memcheck --leak-check=summary --log-file=${VALGRIND_LOG} ${PROG_PATH}")

NO_LOST=$(grep "${NO_LOST_PATTERN}" ${VALGRIND_LOG})
NO_ERROR=$(grep "${NO_ERROR_PATTERN}" ${VALGRIND_LOG})
if [ -z "${NO_LOST}" ] || [ -z "${NO_ERROR}" ]; then
  echo -e "TEST ${test} FAILED\n"

  cat ${VALGRIND_LOG}
  echo -e "\n\nRUN: valgrind --tool=memcheck --leak-check=full \n\n"

  rm -rf ${VALGRIND_LOG}
  exit 1
fi

rm -f ${VALGRIND_LOG}
echo "SUCCESS"
