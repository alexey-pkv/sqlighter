cmake -S . -B build/debug
cmake --build build/debug --target leaks -- -j10
