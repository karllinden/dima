#!/bin/bash

status=0

files=$(git ls-files | grep '\.\(c\|h\)$')
for file in $files; do
    clang-format "${@}" $file
    status=$((status + $?))
done

exit $status
