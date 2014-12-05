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
