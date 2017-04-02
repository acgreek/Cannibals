#pragma once
struct Options {
	int apes=1;
	int missionaries=3;
	int monkeys=2;
};

void readOptions(Options & opt, int argc, char * argv[]);