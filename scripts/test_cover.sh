#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <path-to-executable>"
    exit 1
fi


EXECUTABLE_PATH="$1"


############################
### Delete previous data ###
############################

echo "Removing previous build/coverage directory..."
rm -rf "$EXECUTABLE_PATH/coverage" || { echo "Failed to remove $EXECUTABLE_PATH/coverage"; exit 1; }

echo "Deleting .gcda files..."
find . -name "*.gcda" -delete || { echo "Failed to delete .gcda files"; exit 1; }


########################
### Run Tests ##########
########################

echo "Running sqlighter_tests..."
"$EXECUTABLE_PATH/sqlighter_tests" || { echo "Test execution failed"; exit 1; }


########################
### Generate Reports ###
########################

echo "Generating coverage info..."
lcov --capture --directory "$EXECUTABLE_PATH" --output-file "$EXECUTABLE_PATH/coverage.info"                              || { echo "Failed to generate coverage info"; exit 1; }
lcov --extract "$EXECUTABLE_PATH/coverage.info" '*/sqlighter/*' --output-file "$EXECUTABLE_PATH/sqlighter_coverage.info"  || { echo "Failed to filter coverage info"; exit 1; }
genhtml "$EXECUTABLE_PATH/sqlighter_coverage.info" --output-directory "$EXECUTABLE_PATH/coverage"                         || { echo "Failed to generate HTML report"; exit 1; }

echo "Script completed successfully."