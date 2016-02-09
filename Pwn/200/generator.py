#!/usr/bin/python
from os import popen
from os import mkdir
from sys import argv
from random import random as rand
N = int(argv[1])

for t in range(N):
	sec=""
	L = 32
	for i in range(L):
		sec += chr(ord('a')+int(rand()*26))
	sec = popen("echo -n %s | md5sum"%sec).read().split(' ')[0]
	rep = "cat pwn200.c | sed 's/1337/%d/' | sed 's/FLAG/%s/'"%(3000+t, sec)
	source= popen(rep).read()
	
	mkdir('team%d'%t)
	f = file('team%d/pwn200.c'%t, 'w+')
	f.write(source)
	
	popen("echo 'INSERT INTO `flags` (`teamID`, `taskID`, `flag`) VALUES (%d, 17, '\"'\"%s\"'\"');\' >> pwn200.sql"%((t+1), sec))
