# Commands for building library with library CMakeLists.txt

cmake -S . -B build

cmake --build build

# Install build at user .local folder
cmake --install build --prefix "$env:USERPROFILE\.local"