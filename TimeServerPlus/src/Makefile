CXXFLAGS += --std=c++11 -O2
LIBS = -lpthread
SRC = $(shell echo core/*.cpp contrib/*.cpp)
MODULE_OBJ = $(MODULE_SRC:%.cpp=%.o)

all: modules
	g++ $(CXXFLAGS) $(SRC) modules/libtspmodules.so TimeServerPlus.cpp -o main.out $(LIBS)

modules:
	cd modules && make

.PHONY: clean
clean:
	rm main.out

.PHONY: cleano
cleano:
	rm core/*.o contrib/*.o modules/*.o

.PHONY: cleanso
cleanso:
	rm core/*.so contrib/*.so modules/*.so *.so
