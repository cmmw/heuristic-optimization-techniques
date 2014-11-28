#include <ctime>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <cstdio>
#include <string>

#include "Graph.h"
#include "ConstHeu.h"
#include "LNS.h"

using namespace tcbvrp;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " " << "--instanceFile [instanceFile]" << std::endl;
		return -1;
	}

	// get_opt start
	int c;
	std::istringstream arg;
	std::string instanceFile;

	while (1)
	{
		int option_index = 0;
		static struct option long_options[] =
				{
						{ "instanceFile", required_argument, 0, 'i' },
						{ 0, 0, 0, 0 }
				};

		c = getopt_long(argc, argv, "",
				long_options, &option_index);
		if (c == -1)
			break;
		arg.clear();

		switch (c)
		{
		case 'i':
			instanceFile = optarg;
			break;
		default:
			printf("?? getopt returned character code 0%o ??\n", c);
		}
	}

	if (optind < argc)
	{
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
	// getopt end

	Graph graph = Graph::createGraph(instanceFile);

	graph.printGraph();

	Solution sol(graph.getAdjacencyMatrix());
	ConstHeu cons(&sol, graph);
	cons.solve();

	/*Print results*/
	std::cout << "Initial solution" << std::endl;
	sol.printSolution();
	std::cout << sol.getNumberOfTours() << " tours created, allowed: " << graph.getNumberOfVehicles() << std::endl;

	LNS lns(&sol, graph);
	clock_t begin = clock();
	lns.solve();
	clock_t end = clock();

	/*Print results*/
	std::cout << std::endl << "VNS solution" << std::endl;
	sol.printSolution();
	std::cout << sol.getNumberOfTours() << " tours created, allowed: " << graph.getNumberOfVehicles() << std::endl;

	double ms = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Took " << ms << " seconds" << std::endl;
	return 0;
}
