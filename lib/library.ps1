# Commands for building library with library CMakeLists.txt
Remove-Item -Recurse -Force .\build\

cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release

cmake --build build

# Install build at user graphiti folder
cmake --install build --prefix "$env:USERPROFILE\graphiti"