#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int three = 0xb588;
int not_valid() {
	printf("not valid :(\n");
	return 0;
}

int debugger_detected() {
	printf("debugger detected\n");
	return 0;
}

int checksum_failed() {
	printf("checksum failed\n");
	return 0;
}

int valid() {
	printf("IP Address IS VALID !!! :)))\n");
	return 0;
}

int main() {
	int i;
	char ipv6[43];
	fgets(ipv6,40,stdin);
	time_t global_start = time(NULL);
	int l = strlen(ipv6);
	int *five = (int*)malloc(sizeof(int));
	*five=0xdd87;
	time_t end_time;
	while(ipv6[l-1] == '\n') {
		l--;
		ipv6[l] = 0;
	}

	if(l != 39) {
		return not_valid();
	}
	for(i=0;i<l;i++) {
		if((i+1)%5) { // other
			end_time = time(NULL);			
			if(end_time - global_start > 1) {
				return debugger_detected();
			}
			if(!((ipv6[i]>='a' && ipv6[i] <= 'f') || (ipv6[i]>='0' && ipv6[i] <='9')))
				return not_valid();
			end_time = time(NULL);
			if(end_time - global_start > 1) {
				return debugger_detected();
			}
			else {
				if(ipv6[i]>='a' && ipv6[i] <='f') {
					end_time = time(NULL);					
					if(end_time - global_start > 1) {
						return debugger_detected();
					}
					ipv6[i] -= 'a';
					ipv6[i] += 10;
				} else {
					end_time = time(NULL);					
					if(end_time - global_start > 1) {
						return debugger_detected();
					}
					ipv6[i] -= '0';
				}
			}
			end_time = time(NULL);
			if(end_time - global_start > 1) {
				return debugger_detected();
			}
		} else { // :
			end_time = time(NULL);			
			if(end_time - global_start > 1) {
				return debugger_detected();
			}
			if(ipv6[i] != ':')
				return not_valid();
		}
		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
	}

	int arr[8];
	for(i=0;i<8;i++) {
		arr[i] = ipv6[i*5]*4096 + ipv6[i*5+1]*256 + ipv6[i*5+2]*16 + ipv6[i*5+3];
		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
	}


	int xored = 0;
	for(i=0;i<7;i++) {
		global_start = time(NULL);
		xored ^= arr[i];
		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
	}

	if(xored != arr[7]) {
		return checksum_failed();
	}

	end_time = time(NULL);
	if(end_time - global_start > 1) {
		return debugger_detected();
	}


	xored = 0;
	for(i=0;i<8;i++) {
		xored ^= arr[i];
		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}

	}
	for(i=0;i<8;i++) {
		xored ^= arr[i];
		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}

	}

	if(xored) {
		return checksum_failed();
	}

	__asm__("pushq %rax");
	__asm__("addq $32,%rax");
	__asm__("pushq %rbx");
	__asm__("addq %rax,%rbx");
	__asm__("popq %rbx");
	__asm__("popq %rax");

	if(fork()!=0) {
		xored = 0;
		int (*a)() = &checksum_failed;
		long b = (long)a;
		for(i=0;i<8;i++) {
			xored ^= *((char*)b);
			b+=1;
			end_time = time(NULL);
			if(end_time - global_start > 1) {
				return debugger_detected();
			}

		}
		b = (long)a;
		for(i=0;i<8;i++) {
			xored ^= *((char*)b);
			b+=1;
			end_time = time(NULL);
			if(end_time - global_start > 1) {
				return debugger_detected();
			}

		}

		if(xored) {
			return checksum_failed();
		}

		// arr[1] arr[3] arr[5] CONST
		// arr[0] = 1 XOR 3 XOR 5
		// arr[2] = 1 XOR 0
		// arr[4] = 0 XOR 1 XOR 2 XOR 3
		// arr[6] = 4 XOR 5
		// arr[7] = XOR(0..6)

		int one = 0xfd1f;
		//one local
		//three global
		//five pointer
		if(arr[1] - one) {
			return checksum_failed();
		}
		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
		__asm__("pushq %rax");
		__asm__("addl $0xdeffbeef,%eax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("movq $0xdeffbeef,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		if(arr[2] ^ arr[1] ^ arr[0]) {
			return checksum_failed();
		}

		struct {
			long **a[3];
			char b;
		} STR;

		STR.b = 'c';
		STR.a[0] = (long**)12;
		STR.a[1] = (long**)17;
		STR.a[2] = (long**)23;

		if((((char)(STR.a[0])) ^ ((char)(STR.a[1])) ^ ((char)(STR.a[2])) ^ (STR.b))-105) {
			return checksum_failed();
		}

		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
		if(arr[3] - three) {
			return checksum_failed();
		}

		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
		if(arr[0] ^ arr[1] ^ arr[3] ^ arr[5]) {
			return checksum_failed();
		}

		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
		if(arr[4] ^ arr[0] ^ arr[1] ^ arr[2] ^ arr[3]) {
			return checksum_failed();
		}

		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
		if(arr[4] ^ arr[5] ^ arr[6]) {
			return checksum_failed();
		}

		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}
		if(arr[5] - (*five)) {
			return checksum_failed();
		}

		end_time = time(NULL);
		if(end_time - global_start > 1) {
			return debugger_detected();
		}

	} else {
		xored = 0;
		int (*a)() = &fork;
		long b = (long)a;
		for(i=0;i<17;i++) {
			xored ^= *((char*)b);
			b+=1;
			end_time = time(NULL);
			if(end_time - global_start > 1) {
				return debugger_detected();
			}

		}
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		__asm__("pushq %rax");
		__asm__("addq $32,%rax");
		__asm__("pushq %rbx");
		__asm__("addq %rax,%rbx");
		__asm__("popq %rbx");
		__asm__("popq %rax");
		b = (long)a;
		for(i=0;i<17;i++) {
			xored ^= *((char*)b);
			b+=1;
			end_time = time(NULL);
			if(end_time - global_start > 1) {
				return debugger_detected();
			}

		}

		if(xored) {
			return checksum_failed();
		}
		return 0;
	}

	end_time = time(NULL);
	if(end_time - global_start > 1) {
		return debugger_detected();
	}

	return valid();
}
