CC = g++
INC_DIR = "C:\MinGW\freeglut\include"
VPATH = object/ bitmap/ graphic/ tm/

OBJPATH = obj/
OBJFLAGS = -c -pthread -o
EXEFILE = symmetry
TESTFILE = testfile


bygg: 
	make -s $(OBJPATH)prefix.o
	make -s $(OBJPATH)orientation.o
	make -s $(OBJPATH)graph2d.o
	make -s $(OBJPATH)point.o
	make -s $(OBJPATH)main.o
	make -s $(OBJPATH)gui.o
	make -s $(OBJPATH)comm.o
	make -s $(OBJPATH)symmetryObject.o
	make -s $(OBJPATH)glutThread.o
	make -s $(OBJPATH)others.o
	make -s $(OBJPATH)symmetryRelaxation.o
	make -s $(OBJPATH)camera.o
	$(CC) -o $(EXEFILE).exe $(OBJPATH)*.o -L "C:\MinGW\freeglut\lib" -l freeglut -lgdi32 -lopengl32 -lglu32 -lpthreadGC2

$(OBJPATH)main.o: main.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)main.o main.cpp -I $(INC_DIR)

$(OBJPATH)gui.o: gui/gui.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)gui.o gui/gui.cpp -I $(INC_DIR)

$(OBJPATH)comm.o: comm/comm.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)comm.o comm/comm.cpp -I $(INC_DIR)

$(OBJPATH)glutThread.o: glutThread.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)glutThread.o glutThread.cpp -I $(INC_DIR)

$(OBJPATH)glutThread2.o: glutThread2.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)glutThread2.o glutThread2.cpp -I $(INC_DIR)

$(OBJPATH)graph2d.o: graph/graph2d.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)graph2d.o graph/graph2d.cpp -I $(INC_DIR)

$(OBJPATH)point.o: symmetryObject/point.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)point.o symmetryObject/point.cpp -I $(INC_DIR)

$(OBJPATH)prefix.o: symmetryObject/prefix.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)prefix.o symmetryObject/prefix.cpp -I $(INC_DIR)

$(OBJPATH)orientation.o: symmetryObject/orientation.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)orientation.o symmetryObject/orientation.cpp -I $(INC_DIR)

$(OBJPATH)symmetryObject.o: symmetryObject/symmetryObject.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)symmetryObject.o symmetryObject/symmetryObject.cpp -I $(INC_DIR)

$(OBJPATH)others.o: symmetryObject/others.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)others.o symmetryObject/others.cpp -I $(INC_DIR)

$(OBJPATH)symmetryRelaxation.o: symmetryObject/symmetryRelaxation/symmetryRelaxation.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)symmetryRelaxation.o symmetryObject/symmetryRelaxation/symmetryRelaxation.cpp -I $(INC_DIR)

$(OBJPATH)camera.o: graph/camera.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)camera.o graph/camera.cpp -I $(INC_DIR)



tests: 
	make -s $(OBJPATH)prefix.o
	make -s $(OBJPATH)orientation.o
	make -s $(OBJPATH)point.o
	make -s $(OBJPATH)symmetryObject.o
	make -s $(OBJPATH)others.o
	make -s $(OBJPATH)test.o
	make -s $(OBJPATH)testSymmetryObject.o
	$(CC) -o $(TESTFILE).exe $(OBJPATH)*.o 


$(OBJPATH)testSymmetryObject.o: test/testSymmetryObject.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testSymmetryObject.o test/testSymmetryObject.cpp -I $(INC_DIR)

$(OBJPATH)test.o: test/test.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)tests.o test/test.cpp -I $(INC_DIR)



clean: 
	rm -f $(OBJPATH)*.o *.exe