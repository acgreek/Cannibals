#include "options.h"
#ifdef _WIN32
#else 
#include <getopt.h>
#endif

void readOptions(Options & opt, int argc, char * argv[]) {
#ifdef _WIN32
#else 
	while (1) {
		static struct option long_options[] = {
			{ "apes",     required_argument, 0,  0 },
			{ "missionaries",  required_argument, 0,  0 },
			{ "monkeys", required_argument,       0,  0 },
			{ 0,         0,                 0,  0 }
		};
		int option_index = 0;
		char c = getopt_long(argc, argv, "a:m:M:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			switch (option_index) {
			case 0: opt.apes = atoi(optarg); break;
			case 1: opt.missionaries = atoi(optarg); break;
			case 2: opt.monkeys = atoi(optarg); break;
			}
			break;
		case 'a': opt.apes = atoi(optarg); break;
		case 'M': opt.missionaries = atoi(optarg); break;
		case 'm': opt.monkeys = atoi(optarg); break;
		}
	}
#endif

}