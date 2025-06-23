cmake -S . -B build

cmake --build build

cmake --install build --prefix "$env:USERPROFILE\.local"