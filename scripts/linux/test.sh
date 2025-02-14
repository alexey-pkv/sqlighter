#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <path-to-executable>"
    exit 1
fi


EXECUTABLE_PATH="$1"


echo "Running sqlighter_tests"


"$EXECUTABLE_PATH/bin/sqlighter_tests" || { echo "Test execution failed"; exit 1; }
