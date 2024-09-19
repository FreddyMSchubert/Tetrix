brew install raylib
gcc -o test raylibtest.c -I/opt/homebrew/Cellar/raylib/5.0/include -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL
./test
