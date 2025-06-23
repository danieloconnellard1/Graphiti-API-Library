# Commands for building library with library CMakeLists.txt

cmake -S . -B build

cmake --build build

cmake --install build --prefix "$env:USERPROFILE\.local"