import os

f = open('instances-list', 'w')

numberOfTimes = 30

for x in range(0, numberOfTimes):
            instanceFile = "./Instances/tcbvrp_700.prob\n"
            f.write(instanceFile)  
