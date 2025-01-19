#!/bin/bash


echo "Checking for leaks"


valgrind --leak-check=full --error-exitcode=1 ./build/bin/sqlighter_tests || { echo "Execution failed"; exit 1; }
