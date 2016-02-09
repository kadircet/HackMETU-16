#!/usr/bin/python2
class cpu:
	def __init__(self, code, inp, key):
		self.code = code.split('\n')
		self.pc=0
		self.inp = inp
		self.key = key
		self.mem = [0]*128
		self.reg = [0]*128
		for i in range(8):
			self.mem[i] = ord(self.inp[i])
		for i in range(16):
			self.mem[8+i] = ord(self.key[i])
	
	def bitsum(self, vec):
		res = vec & 1
		for i in range(1, 32):
			res += ((vec & (1 << i)) >> i) & 1
		return res
			
	def proc(self, op):
		if op[0]=='[':
			r = int(op[1:-1])
			return reduce(lambda x,y:256*x+y, self.mem[r:r+4])
		elif op[0]=='r':
			r = int(op[1:])
			return self.reg[r]
		else:
			return int(op, 16)
	
	def put(self, op, val):
		if op[0]=='[':
			r = int(op[1:-1])+3
			for i in range(4):
				self.mem[r-i]=val%256
				val/=256
		elif op[0]=='r':
			r = int(op[1:])
			self.reg[r]=val
			
	def execute(self):
		while self.pc<len(self.code):
			inst = self.code[self.pc].split(' ')
			opcode = inst[1]
			diff = 0
			if opcode=="xor":
				op1 = self.proc(inst[2])
				op2 = self.proc(inst[3])
				old = op1
				op1 = op1^op2
				diff = old^op1
				self.put(inst[2], op1)
			elif opcode=="add":
				op1 = self.proc(inst[2])
				op2 = self.proc(inst[3])
				old = op1
				op1 = (op1+op2)&0xFFFFFFFF
				diff = old^op1
				self.put(inst[2], op1)
			elif opcode=="shl":
				op1 = self.proc(inst[2])
				op2 = self.proc(inst[3])
				old = op1
				op1 = (op1<<op2)&0xFFFFFFFF
				diff = old^op1
				self.put(inst[2], op1)
			elif opcode=="mov":
				op1 = self.proc(inst[2])
				op2 = self.proc(inst[3])
				old = op1
				op1 = op2
				diff = old^op1
				self.put(inst[2], op1)
			elif opcode=="shr":
				op1 = self.proc(inst[2])
				op2 = self.proc(inst[3])
				old = op1
				op1 = op1>>op2
				diff = old^op1
				self.put(inst[2], op1)
			elif opcode=="cmp":
				op1 = self.proc(inst[2])
				op2 = self.proc(inst[3])
				old = self.proc("r127")
				op1 = op1-op2
				diff = old^op1
				self.put("r127", op1)
			elif opcode=="jne":
				op1 = self.proc(inst[2])
				old = self.pc
				if self.reg[127]!=0:
					self.pc=op1-1
					diff = old^op1
			elif opcode=="hlt":
				break
			self.pc+=1
			print self.bitsum(diff)

from sys import argv
from os import popen
keys = """b026324c6904b2a9cb4b88d6d61c81d1
26ab0db90d72e28ad0ba1e22ee510510
6d7fce9fee471194aa8b5b6e47267f03
48a24b70a0b376535542b996af517398
1dcca23355272056f04fe8bf20edfce0
9ae0ea9e3c9c6e1b9b6252c8395efdc1
84bc3da1b3e33a18e8d5e1bdd7a18d7a
c30f7472766d25af1dc80b3ffc9a58c7
7c5aba41f53293b712fd86d08ed5b36e
31d30eea8d0968d6458e0ad0027c9f80
166d77ac1b46a1ec38aa35ab7e628ab5
2737b49252e2a4c0fe4c342e92b13285
aa6ed9e0f26a6eba784aae8267df1951
367764329430db34be92fd14a7a770ee
8c9eb686bf3eb5bd83d9373eadf6504b
5b6b41ed9b343fed9cd05a66d36650f0
4d095eeac8ed659b1ce69dcef32ed0dc
cf4278314ef8e4b996e1b798d8eb92cf
3bb50ff8eeb7ad116724b56a820139fa
dbbf8220893d497d403bb9cdf49db7a4
fe9d26c3e620eeb69bd166c8be89fb8f
2fc57d6f63a9ee7e2f21a26fa522e3b6
2a53da1a6fbfc0bafdd96b0a2ea29515
7c67493bd72ceff21059c3d924d17518
2a52a5e65fc3c43f409550dfad1f904f
b0771132ab2531a40c9941375ed8e290
66a7c1d5cb75ef2542524d888fd32f4a
51a6d96331d5eaa300358c7a0faf168d
22cab69bca05d296a2d779a52cdee643
d5b4c7d9b06b60a7846c4529834c9812
4f89a0f6c113ae3ff279af1e6c6286bb
bb743fc2a7213949f25593c51cbee64f
4fbafd6948b6529caa2b78e476359875
fd1bc138d22d4f78150c6e808345c2cc
649ee93d50739c656e94ec88a32c7ffe
fa84f696e31d07f55cd45cc3c9e52f3b
87aa60e6e440a6656bc74eb20476d662
bda81ba88c634b46394ead43aff31ad5
59885ebc737617addaaf0cb8090203fc
90e2a51705594d033a3abe9d77b2b7ad
21fa2e849284bf7681c37a8b565bd934
50a2fabfdd276f573ff97ace8b11c5f4
f0287f33eba7192e2a9c6a14f829aa1a
e760668b6273d38c832c153fde5725da
93e50d72ab294091a81d828459019eba
d3a57c7e953913944c76009357469568
4a5c26d88c514a23bf15654abd83adb1
08c61f3fd48f12fa7c88a7f5fd01df3d
a75f5a8337c6699916182b6f7c10d7a9
6eb5cefde6fcb8463cea70880a44eb98""".split('\n')
key = keys[int(argv[2])]
#key = "".join([chr(x) for x in range(16)])
CPU = cpu(open("renc.asm", "r").read(), argv[1], key)

CPU.execute()

