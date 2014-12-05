import os

f = open('instances-list', 'w')

proportionFixedCells = 0
timeLimit = 30

while proportionFixedCells < 105:
      for x in range(1, 21):
            prename = str(proportionFixedCells) + "/";
            instanceFile = prename + "puzzle" + str(x) + ".txt"
            outFile = "./Instances/" + prename + "out" + str(x) + ".txt"
            logFile = "./Instances/" + prename + "log" + str(x) + ".txt"
            f.write(instanceFile + " --outfile " + outFile + " --logfile " + logFile + " --timeLimit " + str(timeLimit) + "\n") 
            
      
      proportionFixedCells+=5
