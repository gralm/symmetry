CC = g++
INC_DIR = "C:\MinGW\freeglut\include"
VPATH = object/ bitmap/ graphic/ tm/

OBJPATH = obj/
OBJFLAGS = -c -pthread -o 
EXEFILE = symmetry

bygg: 
	make -s $(OBJPATH)main.o
	make -s $(OBJPATH)comm.o
	make -s $(OBJPATH)glutThread.o
	$(CC) -o $(EXEFILE).exe $(OBJPATH)*.o -L "C:\MinGW\freeglut\lib" -l freeglut -lgdi32 -lopengl32 -lglu32 -lpthreadGC2

$(OBJPATH)main.o: main.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)main.o main.cpp -I $(INC_DIR)

$(OBJPATH)comm.o: comm/comm.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)comm.o comm/comm.cpp -I $(INC_DIR)

$(OBJPATH)glutThread.o: glutThread.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)glutThread.o glutThread.cpp -I $(INC_DIR)


clean: 
	rm -f $(OBJPATH)*.o $(EXEFILE).exe