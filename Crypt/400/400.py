from gmpy2 import mpz
import gmpy2
from Crypto.Util import number
import random
import sys

def generate():
	primes = []
	composites=[]
	for i in range(1000):
		primes.append(number.getPrime(1024))
	for i in range(1,1000):
		composites.append(primes[i-1]*primes[i])

	random.shuffle(composites)
	print composites
	sys.stderr.write(str(composites[0])+"\n")

#foundP=[]
#for i in range(999):
#	for j in range(999):
#		if i==j:
#			continue
#		p=gmpy2.gcd(composites[i], composites[j])
#		if p>1:
#			foundP.append(p)
#			foundP.append(composites[i]/p)
p=mpz(136879539319049556687906660518818230892898930161803757032023216914816138786640835527709801513812544955371909036683345095393654530286535681014282249961430979423188074876650524941553413026936533838281136054381984335046004532979371795539304950409648988622214685858522835884068060522877482116306511589614367098143)
q=mpz(93488094760521745490867573600981167326506824062259590701758412961704736978160741035266235873896241059184566340237764070264386769534675426962527810096326138228279252769460632085480075846992461795796507574197925729088472207192317242717765702276177009823779478564206704150744597327822329038606827943300414102837)
N=p*q
e=65537
d=gmpy2.invert(e, N-(p+q-1))

def encrypt(data):
	data=data.encode("hex")
	data=mpz(data, 16)
	return gmpy2.powmod(data, e, N)

def decrypt(data):
	data = gmpy2.powmod(data, d, N)
	data = hex(data)[2:]
	return data.decode("hex")

import base64
from Crypto import Random
from os import popen
from sys import argv
for i in range(int(argv[1])):
	rnd = base64.b64encode(Random.new().read(16))
	rnd=popen("echo -n %s | md5sum"%rnd).read().split(' ')[0]
	print rnd, encrypt(rnd)

