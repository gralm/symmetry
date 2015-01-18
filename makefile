CC = g++
INC_DIR = "C:\MinGW\freeglut\include"
VPATH = object/ bitmap/ graphic/ tm/

OBJPATH = obj/
OBJFLAGS = -c -pthread -o 
EXEFILE = symmetry

bygg: 
	make -s $(OBJPATH)point.o
	make -s $(OBJPATH)prefix.o
	make -s $(OBJPATH)orientation.o
	make -s $(OBJPATH)graph2d.o
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

$(OBJPATH)glutThread2.o: glutThread2.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)glutThread2.o glutThread2.cpp -I $(INC_DIR)

$(OBJPATH)graph2d.o: graph/graph2d.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)graph2d.o graph/graph2d.cpp -I $(INC_DIR)

$(OBJPATH)point.o: graph/orientation/point.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)point.o graph/orientation/point.cpp -I $(INC_DIR)

$(OBJPATH)prefix.o: graph/orientation/prefix.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)prefix.o graph/orientation/prefix.cpp -I $(INC_DIR)

$(OBJPATH)orientation.o: graph/orientation/orientation.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)orientation.o graph/orientation/orientation.cpp -I $(INC_DIR)

onlyglut:
	make -s $(OBJPATH)glutThread2.o
	$(CC) -DONLYGLUT -o onlyglut2.exe $(OBJPATH)glutThread2.o -L "C:\MinGW\freeglut\lib" -l freeglut -lopengl32 -lglu32

clean: 
	rm -f $(OBJPATH)*.o $(EXEFILE).exe