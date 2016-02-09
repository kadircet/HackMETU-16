0  xor eax eax
1  add eax 0x9e
2  shl eax 8
3  add eax 0x37
4  shl eax 8
5  add eax 0x79
6  shl eax 8
7  add eax 0xb9
8  xor rcx rcx
9  xor rbx rbx
10  mov r8d [8]
11  mov r9d [12]
12  mov r10d [16]
13  mov r11d [20]
14  mov r12d [0]
15  mov r13d [4]
16  loop: add ebx eax
	mov edx, r13d
17  shl edx 4
18  add edx r8d
	mov r14d ebx
19  add r14d r13d
	mov r15d r13d
20  shr r15d 5
21  add r15d r9d
22  xor r15d r14d
23  xor r15d edx
24  add r12d r15d
	mov edx r12d
25  shl edx 4
26  add edx r10d
	mov r14d ebx
27  add r14d r12d
	mov r15d r12d
28  shr r15d 5
29  add r15d r11d
30  xor r15d r14d
31  xor r15d edx
32  add r13d r15d# done with line 2! ; 01d17020
33  mov r16d 32# for compare ; 20100020
34  add eax 1# the counter ; 214a0001
35  cmp r16d eax 
	jne 17
