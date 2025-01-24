cmake -S . -B build/cover -DSQLIGHTER_BUILD_FOR_COVER=true
cmake --build build/cover --target cover -- -j10

echo "file://$(dirname "$(realpath "$0")")/build/cover/coverage/index.html"