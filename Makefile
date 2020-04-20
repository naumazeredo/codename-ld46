OUT_EXE = ld46
TARGET = x64
CC =  g++

FLAGS = -std=c++17

DEFS = -D_REENTRANT \
			 -DIMGUI_IMPL_OPENGL_LOADER_GL3W \
			 -DSTBI_ONLY_PNG \
			 -DSTB_IMAGE_IMPLEMENTATION

DEPS_LIB = deps/lib
DEPS_INC = deps/inc
DEPS_SRC = deps/src

#SDL2
SDL2_INC = /usr/include/SDL2

#GL GL3W
GL_SRC = ${DEPS_SRC}/GL

#ImGUI
IMGUI_SRC = ${DEPS_SRC}/imgui
IMGUI_INC = ${DEPS_INC}/imgui

LIBS = -lSDL2 \
			 -lSDL2_mixer \
			 -lGL \
			 -ldl

INCS = -I${DEPS_INC} \
			 -I${SDL2_INC} \
			 -I${IMGUI_INC}

DEP_SRCS = ${IMGUI_SRC}/imgui*.cpp \
					 ${GL_SRC}/gl3w.c

SRCS = ${DEP_SRCS} \
			 src/main.cpp \
			 src/debug.cpp \
			 src/render.cpp \
			 src/input.cpp \
			 src/audio.cpp \
			 src/player.cpp \
			 src/item.cpp \
			 src/time.cpp \
			 src/geom.cpp \
			 src/shop.cpp \
			 src/game.cpp \
			 src/physics.cpp \
			 src/enemy.cpp \
			 src/king.cpp \
			 src/texture.cpp


OBJS = $(SRCS:src/%.cpp=${OUT_DIR}/%.o)
#OBJS = $(patsubst src/%.cpp,${OUT_DIR}/%.o,$(SRC))

FILES = assets

OUT_DIR = build

all: out_dir objs
	${CC} ${OBJS} ${DEFS} ${FLAGS} ${INCS} ${LIBS} -o ${OUT_DIR}/${OUT_EXE}

objs: ${OBJS}

${OUT_DIR}/%.o: src/%.cpp
	${CC} ${DEFS} ${FLAGS} ${INCS} ${LIBS} -c $< -o $@

run: all
	./${OUT_DIR}/${OUT_EXE}

clean:
	rm -rf ${OUT_DIR}

out_dir:
	mkdir -p ${OUT_DIR}
	cp -r ${FILES} ${OUT_DIR}

#TODO:
#  installation of libsdl2-dev
