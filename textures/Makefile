OBJS = simplex_noise.c random.c square_root.c

CC = gcc

INCLUDE_PATHS = -IC:\SDL\SDL2-2.0.8\i686-w64-mingw32\include\SDL2

LIBRARY_PATHS = -LC:\SDL\SDL2-2.0.8\i686-w64-mingw32\lib

COMPILER_FLAGS = -Wall

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

OBJ_NAME = main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)