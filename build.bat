mkdir bin
clang++ src/Engine.cpp  src/Game.cpp ^
    -o bin/game.exe ^
    -std=c++17 ^
    -Wall -Wextra -Wno-c99-designator -Wno-reorder-init-list -Wno-microsoft-enum-forward-reference ^
    -I libs/include ^
    -I shaders/include 
bin\game.exe