CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./ -std=c++11

BIN=ass3_hierarchical_modelling
SRCS=ass3_hierarchical_modelling.cpp gl_framework.cpp shader_util.cpp hierarchy_node.cpp texture.cpp
INCLUDES=gl_framework.hpp shader_util.hpp ass3_hierarchical_modelling.hpp hierarchy_node.hpp texture.hpp

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)
