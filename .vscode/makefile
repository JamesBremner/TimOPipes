#source file search paths
VPATH = ./src 

	# compiler include search paths
INCS = -I./include \
	-I../ravencode/pub/src \
	-I../raven-set \
	-I../boost/boost1_78  \
	-I../PathFinder/src

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \
	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static

# folders
ODIR = ./.vscode/obj
RDIR = ../ravencode/pub/src

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ = \
	$(ODIR)/main.o \
	$(ODIR)/cPathFinder.o \
	$(ODIR)/cGraph.o \
	$(ODIR)/cRunWatch.o

### COMPILE

$(ODIR)/cPathFinder.o : ../PathFinder/src/cPathFinder.cpp
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)
$(ODIR)/cGraph.o : ../PathFinder/src/cGraph.cpp
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)
$(ODIR)/cRunWatch.o :
	g++ -o $@ -c ../raven-set/cRunWatch.cpp \
	$(INCS) $(CCflags) 
$(ODIR)/%.o: %.cpp  
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

bats: $(OBJ)
	g++ $(OPT) \
	-o bin/pipes.exe $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

