#!/bin/bash


echo "Running sqlighter_tests"


./build/bin/sqlighter_tests || { echo "Test execution failed"; exit 1; }
