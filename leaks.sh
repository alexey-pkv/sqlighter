cmake -S . -B build/debug
cmake --build build/debug --target leak_check -- -j10