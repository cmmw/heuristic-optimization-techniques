#!/usr/bin/env python

import sys
import subprocess
import time

exeFile = "./tcbvrp_solver"

if len(sys.argv) < 2:
	print "Usage: ./run.py <args for solver>"
	exit();

args = " ".join(sys.argv[1:])
print "Running " + exeFile + " " + args + " 30 times"

cmd = exeFile + " " + args

best = sys.maxint
bestOutput = ""
start = time.time()
for i in range(0, 30):
	sys.stdout.write("\r")
	sys.stdout.write(str(i))
	sys.stdout.flush()
	origOutput = subprocess.check_output(cmd, shell=True)
	output = origOutput.split("\n")
	cost = output[-2]
	if int(cost) < int(best):
		best = cost
		bestOutput = origOutput
duration = time.time() - start

print ""
print "Best solution found:"
print bestOutput
print "Total time: " + str(duration)
	
