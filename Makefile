CXXFLAGS= -ggdb -Wall -Weffc++
all: canable
	
canable: canable.cc

run: canable
	./canable

.PHONY: run
	
