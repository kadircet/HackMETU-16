#!/usr/bin/python
from os import popen
from os import mkdir
from sys import argv
from random import random as rand
N = int(argv[1])
lines=popen("python2 400.py %d"%N).read().split('\n')

for t in range(N):
	l=lines[t].split(' ')
	popen("echo \'INSERT INTO `crypt400` (`teamID`, `target`) VALUES (%d, \"%s\");\' >> crypt400.sql"%((t+1), l[1]))
	popen("echo \'INSERT INTO `flags` (`teamID`, `taskID`, `flag`) VALUES (%d, 14, \"%s\");\' >> crypt400.sql"%((t+1), l[0]))
