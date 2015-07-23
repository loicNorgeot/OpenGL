EXEPATH=executables/
SRCPATH=sources/
BLDPATH=build/

SRCS=shader.cpp controls.cpp context.cpp mesh.cpp
SRC=$(addprefix $(SRCPATH), SRCS)
HEA=$($(SRC):.cpp=.h)
OBJ=$(addprefix $(BLDPATH), $(SRCS:.cpp=.o))

MAINFILE=main
CXX=g++
LIBS=-I /usr/include/  -I /usr/local/include/
CXXFLAGS=$(LIBS) -lGLEW -lglfw3 -lX11 -lXxf86vm -pthread -lXrandr -lXinerama -lXi -lXcursor -lm -lGL -std=c++11
EXEC=$(EXEPATH)main

main : $(OBJ) $(BLDPATH)$(MAINFILE).o
	$(CXX) -o $(EXEPATH)$@ $^ $(CXXFLAGS)

#build/shader.o: sources/shader.cpp sources/shader.h
#	$(CXX) -o build/shader.o -c $< $(CXXFLAGS)

VPATH=$(SRCPATH)
$(BLDPATH)%.o: %.cpp %.h
	$(CXX) -c $< $(CXXFLAGS) -o $@


$(BLDPATH)main.o : $(SRCPATH)$(MAINFILE).cpp $(HEA)
	$(CXX) -o $(BLDPATH)$(MAINFILE).o -c $< $(CXXFLAGS)

clean:
	rm -rf $(BLDPATH)*.o
	rm $(EXEC)	  
