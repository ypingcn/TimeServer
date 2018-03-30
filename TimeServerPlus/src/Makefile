CXXFLAGS += --std=c++11

LIBS = -lpthread

all:load-modules.o config.o core.o utility.o logger.o request.o type.o response.o
	g++ --std=c++11 load-modules.o  config.o core.o request.o response.o type.o utility.o logger.o TimeServerPlus.cpp -o main.out $(LIBS)


debug:load-modules.o config.o core.o utility.o request.o response.o type.o
	g++ -g --std=c++11 load-modules.o  config.o core.o request.o response.o type.o utility.o TimeServerPlus.cpp -o main $(LIBS)

config.o: config.h
type.o: type.h
utility.o: utility.h
logger.o: logger.h

core.o: core.h
request.o: request.h
response.o: response.h



load-modules.o:
	cd module && make && cp load-modules.o ../load-modules.o

clean:
	rm *.o module/*.o main.out
mclean:
	rm module/*.o load-modules.o
