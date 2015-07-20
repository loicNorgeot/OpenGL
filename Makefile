EXEPATH=executables/
SRCPATH=sources/
BLDPATH=build/

VPATH=$(SRCPATH)

SRCS=shader.cpp
SRC=$(addprefix $(SRCPATH), SRCS)
HEA=$($(SRC):.cpp=.h)
OBJ=$(addprefix $(BLDPATH), $(SRCS:.cpp=.o))

MAINFILE=main
CXX=g++
LIBS=-I /usr/include/  -I /usr/local/include/
CXXFLAGS=$(LIBS) -lGLEW -lglfw3 -lX11 -lXxf86vm -pthread -lXrandr -lXinerama -lXi -lXcursor -lm -lGL
EXEC=$(EXEPATH)main

main : $(OBJ) $(BLDPATH)$(MAINFILE).o
	$(CXX) -o $(EXEPATH)$@ $^ $(CXXFLAGS)

build/shader.o: sources/shader.cpp sources/shader.h
	$(CXX) -o build/shader.o -c $< $(CXXFLAGS)


$(BLDPATH)%.o: %.cpp %.h
	$(CXX) -c $< $(CXXFLAGS)


$(BLDPATH)main.o : $(SRCPATH)$(MAINFILE).cpp $(HEA)
	$(CXX) -o $(BLDPATH)$(MAINFILE).o -c $< $(CXXFLAGS)

clean:
	rm -rf $(BLDPATH)*.o
	rm $(EXEC)	  
