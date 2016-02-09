MOD=1099511627791
BASE=152351
a=803499013851
b=782495138428

def pow(b, p, m):
	res=1
	i=1
	while i<=p:
		if p&i:
			res=(res*b)%m
		i*=2
		b=(b*b)%m
	return res%m

def egcd(a, b):
	if a == 0:
		return (b, 0, 1)
	else:
		g, y, x = egcd(b % a, a)
		return (g, x - (b // a) * y, y)

def modinv(a, m):
	g, x, y = egcd(a, m)
	if g != 1:
		raise Exception('modular inverse does not exist')
	else:
		return x % m
		
A = pow(BASE, a, MOD)
B = pow(BASE, b, MOD)
K = pow(B, a, MOD)
print A,B,K==pow(A,b,MOD)

from Crypto import Random
from Crypto.Cipher import AES
import base64

def enc(data, K):
	IV = Random.new().read(16)
	aes = AES.new(K, AES.MODE_CFB, IV)
	return base64.b64encode(IV+aes.encrypt(data))

def dec(data, K):
	data = base64.b64decode(data)
	IV = data[:16]
	aes = AES.new(K, AES.MODE_CFB, IV)
	return aes.decrypt(data[16:])

from os import popen
from sys import argv

"""
K=popen("echo -n %s | md5sum"%K).read().split(' ')[0]
for i in range(int(argv[1])):
	rnd = base64.b64encode(Random.new().read(16))
	rnd=popen("echo -n %s | md5sum"%rnd).read().split(' ')[0]
	print rnd, enc(rnd, K)
#print dec(argv[1], K)
"""

A=903291794923
B=838683142382

found = {}
b=BASE
for i in xrange(2**20):
	found[b]=i+1
	b=(b*BASE)%MOD

b=modinv(pow(BASE, 2**20, MOD), MOD)
b2=A
for i in xrange(2**20):
	if b2 in found:
		a=i*2**20+found[b2]
		break
	b2=(b2*b)%MOD

K=pow(B, a, MOD)
K=popen("echo -n %s | md5sum"%K).read().split(' ')[0]
print dec(argv[1], K)
