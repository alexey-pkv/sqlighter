cmake -S . -B build/debug
cmake --build build/debug --target sqlighter_tests -- -j10
cmake --build build/cover --target leak_check