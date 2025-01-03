rm -rf build/coverage
find . -name "*.gcda" -delete

cd build/bin
./sqlighter_tests


lcov --capture --directory build --output-file build/coverage.info
genhtml build/coverage.info --output-directory build/coverage