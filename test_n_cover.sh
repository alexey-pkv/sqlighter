cmake -S . -B build && cmake --build build --target run_coverage
valgrind --leak-check=full ./build/bin/sqlighter_tests