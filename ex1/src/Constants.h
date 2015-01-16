//Very large neighborhood search parameters
extern int START_REMOVES;
extern int TRIALS_PER_COUNT;
extern int REMOVE_LIMIT;
extern double D;

//Random construction heuristic parameters
//[0,1], 0 = greedy heuristic, 1 = random solution
extern double ALPHA;

//After GRASP_TRIALS runs without finding a better solution grasp will stop
extern int GRASP_TRIALS;


//ACO
extern double ACO_INIT_PHERO;
extern int ACO_ANTS;
extern int ACO_TIMESTEPS;
extern double ACO_EVAP_RATE;
extern double ACO_ALPHA;
extern double ACO_BETA;
extern double ACO_Q;
extern char ACO_phFlag;
