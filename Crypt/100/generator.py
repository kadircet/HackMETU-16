#!/usr/bin/python
from os import popen
from os import mkdir
from sys import argv
from random import random as rand
pl = "echo -n '%s' | sha256sum | md5sum"
N = int(argv[1])

for t in range(N):
	sec=""
	L = 32
	for i in range(L):
		sec += chr(ord('a')+int(rand()*26))
	sec = popen("echo -n %s | md5sum"%sec).read().split(' ')[0]

	res = popen("echo -n %s | sha256sum"%sec).read().split(' ')[0]
	res = popen("echo -n %s | md5sum"%res).read().split(' ')[0]

	popen("echo \'INSERT INTO `crypt100` (`teamID`, `target`) VALUES (%d, \"%s\");\' >> crypt100.sql"%((t+1), sec))
	popen("echo \'INSERT INTO `flags` (`teamID`, `taskID`, `flag`) VALUES (%d, 11, \"%s\");\' >> crypt100.sql"%((t+1), res))
