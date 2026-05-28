g++ forest.cpp -o forest \
-std=c++17 \
-I$(brew --prefix raylib)/include \
-L$(brew --prefix raylib)/lib \
-lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

./forest
