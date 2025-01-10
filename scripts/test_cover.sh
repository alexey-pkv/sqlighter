#!/bin/bash

############################
### Delete previous data ###
############################

echo "Removing previous build/coverage directory..."
rm -rf build/coverage || { echo "Failed to remove build/coverage"; exit 1; }

echo "Deleting .gcda files..."
find . -name "*.gcda" -delete || { echo "Failed to delete .gcda files"; exit 1; }


########################
### Run Tests ##########
########################

echo "Running sqlighter_tests..."
cd build/bin      || { echo "Failed to change directory to build/bin"; exit 1; }
./sqlighter_tests || { echo "Test execution failed"; exit 1; }
cd ../..          || { echo "Failed to navigate back to the project root"; exit 1; }


########################
### Generate Reports ###
########################

echo "Generating coverage info..."
lcov --capture --directory build --output-file build/coverage.info                              || { echo "Failed to generate coverage info"; exit 1; }
lcov --extract build/coverage.info '*/sqlighter/*' --output-file build/sqlighter_coverage.info  || { echo "Failed to filter coverage info"; exit 1; }
genhtml build/sqlighter_coverage.info --output-directory build/coverage                         || { echo "Failed to generate HTML report"; exit 1; }

echo "Script completed successfully."