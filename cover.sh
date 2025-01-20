cmake -S . -B build/cover --DBUILD_FOR_COVER=true
cmake --build build/cover --target cover -- -j10