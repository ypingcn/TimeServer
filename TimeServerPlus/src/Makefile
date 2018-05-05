CC = g++
CXXFLAGS += --std=c++11 -O3
LIBS = -lpthread
SRC = $(shell echo core/*.cpp contrib/*.cpp)
SRC_OBJ = $(SRC:%.cpp=%.o)
MODULE_SO = libtspmodules.so.$(shell grep TSP_MODULE_SONAME modules/load-module.h | awk '{print $$3}')
OTHER_MODULE_SO  = 
FINAL_EXEC_NAME = main.out

all: copyso
	$(CC) $(CXXFLAGS) TimeServerPlus.cpp $(SRC) $(MODULE_SO) $(MODULE_OTHER_SO) -o  $(FINAL_EXEC_NAME) $(LIBS)

copyso: checkso
	cd modules; cp $(MODULE_SO) ../; cd ..

checkso:
	cd modules; if [ ! -f $(MODULE_SO) ]; then make; fi; cd ..;

.PHONY: clean
clean:
	-rm $(FINAL_EXEC_NAME)

.PHONY: cleano
cleano:
	-rm $(SRC_OBJ)
