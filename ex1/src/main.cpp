#include <ctime>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <cstdio>
#include <string>
#include <cstdlib>

#include "Graph.h"
#include "ConstHeu.h"
#include "LNS.h"
#include "RandConstHeu.h"
#include "GRASP.h"

using namespace tcbvrp;

void runConstHeu(Solution* sol, const Graph& graph);
void runRandConstHeu(Solution* sol, const Graph& graph);
void runLNS(Solution* sol, const Graph& graph);
void runGRASP(Solution* sol, const Graph& graph);

int main(int argc, char* argv[])
{
	srand(time(NULL));
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

	//graph.printGraph();

	Solution sol(graph.getAdjacencyMatrix());

	clock_t begin = clock();

//	runConstHeu(&sol, graph);
//	runRandConstHeu(&sol, graph);
//	runLNS(&sol, graph);
	runGRASP(&sol, graph);

	clock_t end = clock();

	/*Print results*/
	sol.printSolution();

	double ms = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Took " << ms << " seconds" << std::endl;
	return 0;
}

void runConstHeu(Solution* sol, const Graph& graph)
{
	std::cout << "Running ConstHeu" << std::endl;
	ConstHeu cons(sol, graph);
	cons.solve();
}

void runRandConstHeu(Solution* sol, const Graph& graph)
{
	std::cout << "Running RandConstHeu" << std::endl;
	RandConstHeu cons(sol, graph);
	cons.solve();
}

void runLNS(Solution* sol, const Graph& graph)
{
	std::cout << "Running LNS" << std::endl;
	ConstHeu cons(sol, graph);
	cons.solve();
	LNS lns(sol, graph);
	lns.solve();
}

void runGRASP(Solution* sol, const Graph& graph)
{
	std::cout << "Running GRASP" << std::endl;
	GRASP grasp(sol, graph);
	grasp.solve();
}
