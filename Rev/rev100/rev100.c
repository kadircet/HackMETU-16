#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *f = "23e0ce017924be955bd0dd379bfc9de4";
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
	fgets(inp,33,stdin);
	int l;
	int i;
	if((l=strlen(inp)) != strlen(f)) {
		return not_valid();
	}

	for(i=0;i<l;i++) {
		if(inp[i] != f[l-1-i])
			return not_valid();
	}
	return valid();
}
