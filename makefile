

CC = g++
CCFLAGS = -Wall
LIBS = #-I'/usr/local/include/SDL2' -I'/usr/include/SDL2' -lSDL2 -L/usr/local/lib -Wl,-rpath='/usr/local/lib' -lSDL2_image -lSDL2_ttf -lm
OUTPUT = bin/SwarmSimulator

FILES = main Vector2d Point2d Agent

OBJ_F = $(addprefix bin/,$(FILES:=.o))
IMG_F = $(addprefix images/parts/,$(IMAGE_FILES:=.png))

main: $(OBJ_F)
	$(CC) $^ -o $(OUTPUT) $(LIBS) $(CCFLAGS)

bin/%.o: src/%.cpp
	$(CC) -c -MD $(LIBS) $(CCFLAGS) $< -o $@

# pull in dependency info for *existing* .o files
-include $(OBJ_F:.o=.d)

test: 
	cd tests && cmake "CMakeLists.txt" && make && bash runAllTests.sh
