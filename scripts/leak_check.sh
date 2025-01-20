#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <path-to-executable>"
    exit 1
fi


EXECUTABLE_PATH="$1"


echo "Checking for leaks"
valgrind --leak-check=full --error-exitcode=1 "$EXECUTABLE_PATH/sqlighter_tests" || { echo "Execution failed"; exit 1; }
