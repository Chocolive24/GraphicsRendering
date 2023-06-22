mkdir bin
clang++ code/main.cpp ^
    -o bin/game.exe ^
    -std=c++17 ^
    -I libs/include ^
    -I shaders/include
bin\game.exe