FILES = src/Brain.cpp src/Vertex.cpp src/main.cpp

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

all : $(FILES)
	g++ $(FILES) -w $(LINKER_FLAGS) -o program