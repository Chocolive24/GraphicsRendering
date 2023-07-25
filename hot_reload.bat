clang++ src/Game.cpp ^
    -o Game.dll -shared ^
    -std=c++17 ^
    -Wall -Wextra -Wno-c99-designator -Wno-reorder-init-list -Wno-microsoft-enum-forward-reference ^
    -I libs/include ^
    -I shaders/include 

clang++ src/Game.cpp ^
    -o Game.dll -shared ^
    -std=c++17 ^
    -Wall -Wextra -Wno-c99-designator -Wno-reorder-init-list -Wno-microsoft-enum-forward-reference ^
    -I libs/include ^
    -I shaders/include 