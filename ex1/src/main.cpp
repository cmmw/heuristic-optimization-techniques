#include <iostream>
#include <sstream>
#include <getopt.h>
#include <cstdio>
#include <string>

#include "Graph.h"
#include "ConstHeu.h"

using namespace tcbvrp;

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " " << "--instanceFile [instanceFile]" << std::endl;
		return -1;
	}

	// get_opt start
	int c;
	std::istringstream arg;
	std::string instanceFile;

	while (1) {
		int option_index = 0;
		static struct option long_options[] = {
				{"instanceFile", required_argument, 0, 'i'},
				{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "",
				long_options, &option_index);
		if (c == -1)
			break;
		arg.clear();

		switch (c) {
		case 'i':
			instanceFile = optarg;
			break;
		default:
			printf("?? getopt returned character code 0%o ??\n", c);
		}
	}

	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
	// getopt end

	Graph graph = Graph::createGraph(instanceFile);

	graph.printGraph();

	Solution sol;
	ConstHeu cons(&sol, graph);
	cons.solve();

	return 0;
}
