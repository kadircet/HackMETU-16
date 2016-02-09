#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* m="\x54\x00\x52\x57\x50\x57\x04\x55\x52\x00\x07\x0f\x01\x01\x07\x53\x52\x55\x03\x0f\x50\x54\x07\x50\x53\x03\x06\x53\x50\x52\x55\x54";

int not_valid() {
	printf("Flag is not valid.\n");
	return 0;
}

int valid() {
	printf("Flag is valid.\n");
	return 0;
}

int main()
{
	char inp[35];
	fgets(inp,35,stdin);
	int l = (int)strlen(inp);
	int i;
	int p = 0x36;
	for(i=l-1;i>=0;i--)
		if(inp[i] == '\n') {
			l--;
			inp[i] = 0;
		} else break;
	if(l != 32) {
		return not_valid();
	}
	
	for(i=0;i<l;i++) {
		if((m[i] ^ p) != inp[i])
			return not_valid();
	}
	return valid();
}
