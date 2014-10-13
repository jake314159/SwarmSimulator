

CC = g++
CCFLAGS = -Wall
LIBS = -I ./gtest-1.7.0/include/gtest #-I'/usr/local/include/SDL2' -I'/usr/include/SDL2' -lSDL2 -L/usr/local/lib -Wl,-rpath='/usr/local/lib' -lSDL2_image -lSDL2_ttf -lm
OUTPUT = bin/SwarmSimulator

FILES = main Vector2d Point2d

OBJ_F = $(addprefix bin/,$(FILES:=.o))
IMG_F = $(addprefix images/parts/,$(IMAGE_FILES:=.png))

main: $(OBJ_F)
	$(CC) $^ -o $(OUTPUT) $(LIBS) $(CCFLAGS)

bin/%.o: src/%.cpp
	$(CC) -c -MD $(LIBS) $(CCFLAGS) $< -o $@

# pull in dependency info for *existing* .o files
-include $(OBJ_F:.o=.d)

test: 
	cd tests && cmake -DCMAKE_CC_COMPILER=gcc -DCMAKE_CXX_COMPILER=gcc -DCMAKE_CC_COMPILER=gcc "CMakeLists.txt" && make && ./runTests
