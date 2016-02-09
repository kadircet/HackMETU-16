#!/usr/bin/python2
#-*- coding:utf-8 -*-

import re
import random

from struct import unpack

from z3 import *


def randstrnp(n):
	while True:
		s = "".join(random.choice(map(chr, range(1, 128))) for i in range(n))
		if "/" not in s:  # this was banned by HTTP server
			return s


def get_data(s):
	# use locally compiled binary if service is down
	if 0:
		import subprocess
		if "\x00" in s or "\x2f" in s:
			raise IndexError("qwe")
		data = subprocess.check_output(["./cpu.py", s]).replace("\x00", "")
		return map(lambda x: int(x), data.split('\n')[:-1])
	else:
		import requests
		from urllib import quote
		cookie={"PHPSESSID":"t1kk5jldrce3fprbgfshpn6246"}
		r = requests.get("https://ctf.0xdeffbeef.com/181344944889e3b6573c2351862092c3.php?inp=" + quote(s), cookies=cookie)
		data = re.findall(r"y:\[.*?\],", r.content)[0]
		arr = eval(data[2:])
		return arr[0]


def bitsum(vec):
	# z3py friendly
	res = vec & 1
	for i in range(1, 32):
		res += ((vec & (1 << i)) >> i) & 1
	return res


known = {}
for i in range(10):
	s = randstrnp(8)
	known[s] = get_data(s)

data = known.values()[0]
k0bitsum = data[10]
k1bitsum = data[11]
k2bitsum = data[12]
k3bitsum = data[13]

print len(known), "collected"

S = Solver()
k0, k1, k2, k3 = BitVecs("k0 k1 k2 k3", 32)
#S.add(k0 & 0xE0E0E0E0 == 0x40404040)
#S.add(k1 & 0xE0E0E0E0 == 0x40404040)
#S.add(k2 & 0xE0E0E0E0 == 0x40404040)
#S.add(k3 & 0xE0E0E0E0 == 0x40404040)
S.add(k0 & 0x80808080 == 0x00000000)
S.add(k1 & 0x80808080 == 0x00000000)
S.add(k2 & 0x80808080 == 0x00000000)
S.add(k3 & 0x80808080 == 0x00000000)
S.add(bitsum(k0) == k0bitsum)
S.add(bitsum(k1) == k1bitsum)
S.add(bitsum(k2) == k2bitsum)
S.add(bitsum(k3) == k3bitsum)

for s, data in known.items():
	v0 = unpack(">I", s[:4])[0]
	v1 = unpack(">I", s[4:8])[0]

	r = [0]*18
	r[0] = 0x9e3779b9
	r[4] = k0
	r[5] = k1
	r[6] = k2
	r[7] = k3
	r[8] = v0
	r[9] = v1
	r[1] = (r[1] + r[0]) & 0xffffffff
	r[3] = (r[9]<<4) & 0xffffffff
	
	r3b = r[3]
	r[3] = (r[3]+r[4]) & 0xffffffff
	S.add(bitsum(r3b^r[3])==data[19])
	
	r10b=r[1]
	r[10] = (r[1]+r[9])&0xffffffff
	S.add(bitsum(r10b^r[10])==data[21])
	
	r[11] = r[9]>>5
	
	r11b=r[11]
	r[11] = (r[11]+r[5])&0xffffffff
	S.add(bitsum(r11b^r[11])==data[24])
	
	r11b=r[11]
	r[11] = r[11]^r[10]
	S.add(bitsum(r11b^r[11])==data[25])
	
	r11b=r[11]
	r[11] = r[11]^r[3]
	S.add(bitsum(r11b^r[11])==data[26])
	
	r8b=r[8]
	r[8] = (r[8]+r[11])&0xffffffff
	S.add(bitsum(r8b^r[8])==data[27])
	
	S.add(bitsum(r[3]^r[8])==data[28])
	r[3]=(r[8]<<4)&0xffffffff
	
	r3b=r[3]
	r[3] = (r[3]+r[6])&0xffffffff
	S.add(bitsum(r3b^r[3])==data[30])
	
	r10b = r[1]
	r[10] = (r[1]+r[8])&0xffffffff
	S.add(bitsum(r10b^r[10])==data[32])
	
	r11b = r[8]
	r[11] = LShR(r[8],5)
	S.add(bitsum(r11b^r[11])==data[34])
	
	r11b=r[11]
	r[11] = (r[11]+r[7])&0xffffffff
	S.add(bitsum(r11b^r[11])==data[35])
	
	r11b=r[11]
	r[11] = r[11]^r[10]
	S.add(bitsum(r11b^r[11])==data[36])
	
	r11b=r[11]
	r[11] = r[11]^r[3]
	S.add(bitsum(r11b^r[11])==data[37])
	
	r9b=r[9]
	r[9] = (r[9]+r[11])&0xffffffff
	S.add(bitsum(r9b^r[9])==data[38])
	
def n2s(n):
	return ("%08x" % n).decode("hex")


print S
print S.check()
m = S.model()
print m
print hex(m[k0].as_long()), n2s(m[k0].as_long())
print hex(m[k1].as_long()), n2s(m[k1].as_long())
print hex(m[k2].as_long()), n2s(m[k2].as_long())
print hex(m[k3].as_long()), n2s(m[k3].as_long())
