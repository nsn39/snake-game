SRCDIR = src
BINDIR = bin

#OBJS specifies which files to compile as part of the project
FILES = game.cpp
OBJS = $(addprefix $(SRCDIR)/,$(FILES))
EXEC = $(BINDIR)/game

##CC specifies which compiler we're using.
CC = g++

##COMPILER_FLAGS specifies the additional compilation options we're using.
##-w suppresses all the warnings.
COMPILER_FLAGS = -w 

##LINKER_FLAGS specifies the libraries we're linking against.
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

##OBJ_NAME specifies the name of our executable.
OBJ_NAME = $(EXEC)

##Target all
all : $(EXEC)

##This is the target that compiles our executable.
$(EXEC) : $(OBJS)
		$(CC) $(OBJS) $(COMPILER_FLAGS) ${LINKER_FLAGS} -o ${OBJ_NAME}