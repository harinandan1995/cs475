OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw3 -lXxf86vm -lX11 -lpthread -lXrandr -lXinerama -lXi -lXcursor
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include

BIN=00_glInit
SRCS=00_glInit.cpp
INCLUDES=

all: $(BIN)


$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS) 

clean:
	rm -f *~ *.o $(BIN)
