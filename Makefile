CXXFLAGS= -ggdb -Wall -Weffc++
all: cannibals
	
canable: cannibals.cc

run: cannibals
	./cannibals

.PHONY: run
	
