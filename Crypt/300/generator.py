#!/usr/bin/python
from os import popen
from os import mkdir
from sys import argv
from random import random as rand
N = int(argv[1])
lines=popen("python2 300.py %d"%N).read().split('\n')

for t in range(N):
	l=lines[t].split(' ')
	popen("echo \'INSERT INTO `crypt300` (`teamID`, `target`) VALUES (%d, \"%s\");\' >> crypt300.sql"%((t+1), l[1]))
	popen("echo \'INSERT INTO `flags` (`teamID`, `taskID`, `flag`) VALUES (%d, 13, \"%s\");\' >> crypt300.sql"%((t+1), l[0]))
