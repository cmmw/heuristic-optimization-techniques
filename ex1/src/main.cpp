#include <ctime>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <csignal>

#include "Graph.h"
#include "ConstHeu.h"
#include "LNS.h"
#include "RandConstHeu.h"
#include "GRASP.h"
#include "ACO.h"
#include "Logger.h"

using namespace tcbvrp;

//Default values for parameters:
int START_REMOVES = 2;
int TRIALS_PER_COUNT = 15;
int REMOVE_LIMIT = 5;
double D = 3.0;
double ALPHA = 0.43;
int GRASP_TRIALS = 5;
bool quit = false;

void runConstHeu(Solution* sol, const Graph& graph);
void runRandConstHeu(Solution* sol, const Graph& graph);
void runLNS(Solution* sol, const Graph& graph);
void runGRASP(Solution* sol, const Graph& graph);
void runACO(Solution* sol, const Graph& graph);
void signalHandler(int signum);
void checkSolution(const Graph& graph, Solution& sol);

int main(int argc, char* argv[])
{
	srand(time(NULL));
	if (argc < 3 || argc > 17)
	{
		std::cout << "Usage: " << argv[0] << " " << "--instanceFile <file>"
				<< "[ --startRemoves <val> --trials <val> --removeLimit <val> --relatedness <val>"
				<< "--greediness <val> --LNS --graspTrials <val> --algo <0,1,2> ]" << std::endl;
		return -1;
	}

	// get_opt start
	int c;
	int algo = 0;
	std::istringstream arg;
	std::string instanceFile;

	while (true)
	{
		int option_index = 0;
		static struct option long_options[] =
				{
						{ "instanceFile", required_argument, 0, 'i' },
						{ "startRemoves", required_argument, 0, 's' },
						{ "trials", required_argument, 0, 't' },
						{ "removeLimit", required_argument, 0, 'l' },
						{ "relatedness", required_argument, 0, 'r' },
						{ "greediness", required_argument, 0, 'g' },
						{ "algo", required_argument, 0, 'a' },					//0 greedy const. heu., 1 vlns, 2 grasp
						{ "graspTrials", required_argument, 0, 'z' },
						{ 0, 0, 0, 0 }
				};

		c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;
		arg.clear();

		switch (c)
		{
		case 'i':
			instanceFile = optarg;
			break;
		case 's':
			arg.str(optarg);
			arg >> START_REMOVES;
			if (START_REMOVES < 0)
			{
				std::cerr << "startRemoves must be > 0" << std::endl;
				return 1;
			}
			break;
		case 't':
			arg.str(optarg);
			arg >> TRIALS_PER_COUNT;
			if (TRIALS_PER_COUNT < 0)
			{
				std::cerr << "trials must be > 0" << std::endl;
				return 1;
			}
			break;
		case 'l':
			arg.str(optarg);
			arg >> REMOVE_LIMIT;
			if (REMOVE_LIMIT < 0)
			{
				std::cerr << "REMOVE_LIMIT must be > 0" << std::endl;
				return 1;
			}
			break;
		case 'r':
			arg.str(optarg);
			arg >> D;
			if (D < 0)
			{
				std::cerr << "relatedness must be > 0" << std::endl;
				return 1;
			}
			break;
		case 'g':
			arg.str(optarg);
			arg >> ALPHA;
			if (ALPHA < 0 || ALPHA > 1)
			{
				std::cerr << "greediness must be in range [0,1]" << std::endl;
				return 1;
			}
			break;
		case 'a':
			arg.str(optarg);
			arg >> algo;
			break;
		case 'z':
			arg.str(optarg);
			arg >> GRASP_TRIALS;
			if (GRASP_TRIALS < 0)
			{
				std::cerr << "graspTrials must be > 0" << std::endl;
				return 1;
			}
			break;
		default:
			std::cerr << "?? getopt returned character code " << std::oct << std::showbase << c << " ??" << std::endl;
			return 1;
		}
	}

	if (START_REMOVES > REMOVE_LIMIT)
	{
		std::cerr << "startRemoves must be <= removeLimit" << std::endl;
		return 1;
	}

	if (instanceFile.empty())
	{
		std::cerr << "Argument --instanceFile <file> missing" << std::endl;
		return 1;
	}

	if (optind < argc)
	{
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
	// getopt end

	LOG << "Set START_REMOVES to " << START_REMOVES;
	LOG << "Set TRIALS_PER_COUNT to " << TRIALS_PER_COUNT;
	LOG << "Set REMOVE_LIMIT to " << REMOVE_LIMIT;
	LOG << "Set D to " << D;
	LOG << "Set ALPHA to " << ALPHA;
	LOG << "Set GRASP_TRIALS to " << GRASP_TRIALS;

	signal(SIGINT, signalHandler);

	Graph graph = Graph::createGraph(instanceFile);
	//graph.printGraph();

	Solution sol(graph.getAdjacencyMatrix());

	clock_t begin = clock();

	switch (algo)
	{
	case 0:
		runConstHeu(&sol, graph);
		break;
	case 1:
		runLNS(&sol, graph);
		break;
	case 2:
		runGRASP(&sol, graph);
		break;
	case 3:
		runACO(&sol, graph);
		break;
	default:
		std::cerr << "Wrong algorithm" << std::endl;
		exit(1);
	}

	clock_t end = clock();

	//checkSolution(graph, sol);

	/*Print results*/
	sol.printSolution();

	double ms = double(end - begin) / CLOCKS_PER_SEC;
	LOG << "Took " << ms << " seconds";

	// official output comes here
	sol.printOfficialSolution();

	// only print total costs for irace
//	std::cout << sol.getTotalCosts() << std::endl;

	return 0;
}

void runConstHeu(Solution* sol, const Graph& graph)
{
	LOG << "Running ConstHeu";
	ConstHeu cons(sol, graph);
	cons.solve();
}

void runRandConstHeu(Solution* sol, const Graph& graph)
{
	LOG << "Running RandConstHeu";
	RandConstHeu cons(sol, graph);
	cons.solve();
}

void runLNS(Solution* sol, const Graph& graph)
{
	LOG << "Running LNS";
	ConstHeu cons(sol, graph);
	cons.solve();
	LNS lns(sol, graph);
	lns.solve();
}

void runGRASP(Solution* sol, const Graph& graph)
{
	LOG << "Running GRASP";
	GRASP grasp(sol, graph);
	grasp.solve();
}

void runACO(Solution* sol, const Graph& graph)
{
	LOG << "Running ACO";
	ACO aco(sol, graph);
	aco.solve();
}

void signalHandler(int signum)
{
	quit = true;
}

void checkSolution(const Graph& graph, Solution& sol)
{
	for (std::vector<Node*>::const_iterator it = graph.getNodes().begin(); it != graph.getNodes().end(); it++)
	{
		(*it)->setVisited(false);
	}

	const std::vector<std::vector<int> >& m = graph.getAdjacencyMatrix();
	for (std::vector<std::vector<Node*> >::iterator tour = sol.getTours().begin(); tour != sol.getTours().end(); tour++)
	{
		for (std::vector<Node*>::const_iterator it = tour->begin(); it != tour->end(); it++)
		{
			for (std::vector<Node*>::const_iterator nodeIt = graph.getNodes().begin(); nodeIt != graph.getNodes().end(); nodeIt++)
			{
				if ((*nodeIt)->getId() == (*it)->getId())
				{
					(*nodeIt)->setVisited(true);
					break;
				}
			}
		}
	}

	for (std::vector<Node*>::const_iterator it = graph.getDemandNodes().begin(); it != graph.getDemandNodes().end(); it++)
	{
		if (!(*it)->getVisited())
		{
			std::cerr << "Solution infeasible: contains unvisited demand nodes!" << std::endl;
			exit(1);
		}
	}

	if (sol.getNumberOfTours() > graph.getNumberOfVehicles())
	{
		std::cerr << "Solution infeasible: contains too many tours!" << std::endl;
		exit(1);
	}
//	std::cerr << "Solution feasible" << std::endl;
}
