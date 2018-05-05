CC = g++
CXXFLAGS += --std=c++11 --shared -fPIC -O3
LIBS = -lpthread
MODULE_SRC = $(shell echo *.cpp)
MODULE_OBJ = $(MODULE_SRC:%.cpp=%.o)
MODULE_SO_NAME = libtspmodules.so.$(shell grep TSP_MODULE_SONAME load-module.h | awk '{print $$3}')
OTHER_MODULE_SO = 

all:
	$(CC) $(CXXFLAGS) $(MODULE_SRC) $(OTHER_MODULE_SO) -o $(MODULE_SO_NAME) $(LIBS)
