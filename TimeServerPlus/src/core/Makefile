CXXFLAGS += --std=c++11
LIBS = -lpthread
MODULE_SRC = $(shell echo *.cpp)
MODULE_OBJ = $(MODULE_SRC:%.cpp=%.o)

all:
	g++ $(CXXFLAGS) -c $(MODULE_SRC) $(LIBS)
	ld -r -s -o libtspcore.so $(MODULE_OBJ)
