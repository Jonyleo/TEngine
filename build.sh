cmake          --no-warn-unused-cli \
               -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
               -DCMAKE_BUILD_TYPE:STRING=Debug \
               -DCMAKE_C_COMPILER:FILEPATH=gcc \
               -DCMAKE_CXX_COMPILER:FILEPATH=g++ \
               -S. \
               -B./build \
               -G Ninja

cmake --build ./build --config Debug --target all --

./build/tangram/tangram 