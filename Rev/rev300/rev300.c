#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>

char *key = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff";
char *success = "key is valid.\n";
char *fail = "key is not valid.\n";
int failed() {
	printf(fail);
	return 0;
}
int valid() {
	printf(success);
	return 0;
}

int main()
{
	char inp[96];
	char hash[17];
	char cur[6];
	int i;

	fgets(inp,96,stdin);

	int l = strlen(inp);

	for(i = l-1 ; i>=0;i--)
		if(inp[i] == '\n') {
			inp[i] = 0;
			l--;
		} else break;

	if(strlen(inp) != 95) {
		return failed();
	}

	for(i=0;i<95;i++) {
		if(i%6 == 5) {
			if(inp[i] != '-')
				return failed();
		}
		else {
			if(!((inp[i] >= 'a' && inp[i] <= 'z') || (inp[i] >= '0' && inp[i]<='9'))) {
				return failed();
			}
		}
	}
	for(i=0;i<16;i++) {
		int j = i*6;
		int p;
		for(p=j;p<j+5;p++) {
			cur[p-j] = inp[p];
		}
		cur[5] = 0;
		MD5(cur,5,hash);
		if((hash[i] != key[i])) {
			return failed();
		}
	}

	return valid();
}
