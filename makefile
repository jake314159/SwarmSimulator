

CC = g++
# -g for debug
CCFLAGS = -Wall -Wno-write-strings -O3
#-Ofast -g
LIBS = -I'/usr/local/include/SDL2' -I'/usr/include/SDL2' -lSDL2 -L/usr/local/lib -Wl,-rpath '/usr/local/lib' -lSDL2_image -lSDL2_ttf -lz 

UNAME := $(shell uname)

ifneq ($(UNAME), Darwin)
LIBS += $(LIBS) -lglut -lGL
endif

ifeq ($(UNAME), Darwin)
LIBS_EXTRA = -framework GLUT -framework OpenGL
endif


OUTPUT = bin/SwarmSimulator


FILES = Vector2d Point2d Agent Simulation Display SDL_functions Environment EnvironmentStore fastMath

OBJ_F = $(addprefix bin/,$(FILES:=.o))
IMG_F = $(addprefix images/parts/,$(IMAGE_FILES:=.png))

SCREENSHOT_DIR = PNG_savesurf/

main: src/main.cpp $(OBJ_F)
	$(CC) $^ -o $(OUTPUT) $(LIBS_EXTRA) $(LIBS) $(CCFLAGS)

DataCollector: src/DataCollector.cpp $(LIBS_EXTRA) $(OBJ_F)
	$(CC) $^ -o bin/DataCollector $(LIBS) $(CCFLAGS)

GA: src/ga.cpp $(OBJ_F)
	$(CC) $^ -o bin/GA $(LIBS_EXTRA) $(LIBS) $(CCFLAGS)

AnalyseSearchSpace: src/AnalyseSearchSpace.cpp $(LIBS_EXTRA) $(OBJ_F)
	$(CC) $^ -o bin/AnalyseSearchSpace $(LIBS) $(CCFLAGS)

bin/%.o: src/%.cpp
	$(CC) -c -MD $(LIBS) $(CCFLAGS) $< -o $@

# pull in dependency info for *existing* .o files
-include $(OBJ_F:.o=.d)

test: 
	cd tests && cmake "CMakeLists.txt" && make && bash runAllTests.sh


