#!/bin/sh
#Generic Script to Solve TCBVRP Instances
TCBVRPPATH="/home1/yourID/whereYouWant"

#Path to your solver
EXECUTABLE="$TCBVRPPATH/YourSolverApp"

#Reduce for testing purposes, but final results need to be based on 30 runs
RUNS=30

#Location for your jobs, do not change
SOLVELOCATION="-l noX"


for SIZE in 10 30 60 90 120 240 400 500 600 700
do
	INSTANCE=$TCBVRPPATH/wherever/tcbvrp_${SIZE}.prob
	OUTPUTFOLDER=$TCBVRPPATH/wherever
	OUTPUTFILEBASE=extralog_${SIZE}

#flags for your application
	FLAGS="$INSTANCE"

	ARRAYFLAG="-t 1-$RUNS"

	JOBSCRIPT=$TCBVRPPATH/wherever/job_${SIZE}.sh

#workaround to get separate output and error logs for every run of an array job
	echo "#!/bin/bash" > $JOBSCRIPT
	echo "#$ -o $TCBVRPPATH/wherever/size$SIZE"-r'$TASK_ID'"o.log" >> $JOBSCRIPT
	echo "#$ -e $TCBVRPPATH/wherever/size$SIZE"-r'$TASK_ID'"e.log" >> $JOBSCRIPT
	echo "" >> $JOBSCRIPT
	cat $TCBVRPPATH/SolveArray.sh >> $JOBSCRIPT

#actually submit the job, you can modify the job name to your liking
	qsub -N "TCBVRP-$SIZE" $EMAILFLAG -r y $SOLVELOCATION $ARRAYFLAG\
	$JOBSCRIPT "$EXECUTABLE" "$FLAGS" $OUTPUTFILEBASE $OUTPUTFOLDER


done

echo "SOLVING JOBS SUBMITTED"


