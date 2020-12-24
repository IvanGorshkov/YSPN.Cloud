#!/usr/bin/env bash

set -e
set -o pipefail

function print_header() {
  echo -e "\n***** ${1} *****"
}

print_header "RUN cppcheck"
if [ "${1}" == "--local" ]; then
  CPPCHECK="cppcheck"
else
  CPPCHECK="./linters/cppcheck/cppcheck"
fi
${CPPCHECK} project --enable=all --error-exitcode=1 -I project/* --suppress=missingIncludeSystem

print_header "RUN cpplint.py"
python2.7 ./linters/cpplint/cpplint.py --extensions=cpp --headers=h,hpp --filter=-runtime/string,-runtime/references,-legal/copyright,-build/include_subdir,-whitespace/line_length,-runtime/threadsafe_fn,-readability/casting project/App/include/* project/App/src/* project/Client/Chunker/include/* project/Client/Chunker/src/* project/Client/Indexer/include/* project/Client/Indexer/src/* project/Client/Watcher/Apple/include/* project/Client/Watcher/Apple/src/* project/Client/Watcher/Linux/include/* project/Client/Watcher/Linux/src/* project/Client/Watcher/include/* project/Databases/internalDB/include/* project/Databases/internalDB/src/* project/Databases/MongoDB/include/* project/Databases/MongoDB/src/* project/Databases/PostgresDB/Base/include/* project/Databases/PostgresDB/Base/src/* project/Databases/PostgresDB/MetaDataDB/include/* project/Databases/PostgresDB/MetaDataDb/src/* project/Network/Client/include/* project/Network/Client/src/* project/Network/Server/include/* project/Network/Server/src/* project/Servers/server/include/* project/Servers/server/src/* project/Servers/storageServer/include/* project/Servers/storageServer/src/* project/Servers/syncServer/include/* project/Servers/syncServer/src/* project/utils/include/structs/* project/utils/include/* project/utils/src/*

print_header "SUCCESS"
