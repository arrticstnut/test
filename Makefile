INC_DIR:= \
	./include/ \
	./include/wordSegmentation/cppjieba/ \
	./include/wordSegmentation/ \
	./include/wordSegmentation/cppjieba/limonp/ \
	./include/tinyxml2/ \
	./include/configuration/ \
	./include/preProcess/ \
#SRC_DIR:= 
#	./src/ \
#	./src/task/ \
#	./src/tcpSer/ \
#	./src/threadPool/ \
#	./src/cacle/ \
#	./src/timer/ \
#	./src/cppjieba/ \
#	./src/configuration/ \

SRCS:=\
	$(wildcard ./src/*.cc) \
	$(wildcard ./src/tinyxml2/*.cc) \
	$(wildcard ./src/configuration/*.cc) \
	$(wildcard ./src/wordSegmentation/*.cc) \
	$(wildcard ./src/preProcess/*.cc) \

OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread

CXX:=g++

CXXFLAGS:= -Wall -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated \
	-g

EXE:=./bin/ser

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
#	rm -rf $(EXE)
	rm -rf $(OBJS)

