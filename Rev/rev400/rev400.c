#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int data_t;
#define BYTES 32
#define ELE_COUNT BYTES/(sizeof(data_t))
typedef data_t vec_t __attribute__ ((vector_size(BYTES)));

typedef union {
	vec_t vec;
	data_t data[ELE_COUNT];
} uni_t;

int not_valid() {
	printf("Key is not valid.\n");
	return 0;
}

int valid() {
	printf("Key is valid\n");
}

int main() {
	uni_t vec_inp,key,res;
	vec_t vec_inp_vec,key_vec,res_vec;
	FILE* fp = fopen("inp.txt","r");
	char inp[50];
	int i;
	short j;
	int len;
	char acc[6];
	fgets(inp,48,fp);
	len = strlen(inp);
	if(inp[len-1] == '\n')
		inp[--len] = 0;
	if(len != 47)
		return not_valid();
	for(i=0;i<=41;i++) {
		if((i+1)%6) {
			if(!(inp[i]>='0' && inp[i] <= '9')) {
				return not_valid();
			}
		} else {
			if(inp[i] != '-')
				return not_valid();
			else {
				for(j=0;j<5;j++)
					acc[j] = inp[i-5+j];
				acc[j] = 0;
				vec_inp.data[(i+1)/6-1] = atoi(acc);
			}
		}
	}
	j=0;
	for(;i<47;i++) {
		if(!(inp[i] >= '0' && inp[i] <= '9'))
			return not_valid();
		acc[j] = inp[i];
		j++;
	}
	acc[j] = 0;
	vec_inp.data[7] = atoi(acc);
	
	for(i=0; i<ELE_COUNT;i++) {
		vec_inp.data[i]^=0xdeffbeef;
	}


	vec_inp_vec = vec_inp.vec;

	key.data[0] = 0xD00D;
	key.data[1] = key.data[0] + 8192; // 0xf00d
	key.data[2] = key.data[1] + (-9487); // 0xcafe
	key.data[3] = key.data[2] + (1295); // 0xd00d 
	key.data[4] = key.data[3] + (-3887); // 0xc0de
	key.data[5] = key.data[4] + (15887); // 0xfeed
	key.data[6] = key.data[5] + (-8256); // 0xdead
	key.data[7] = key.data[6] + (-37776); // 0x4b1d


	key_vec = key.vec;

	res_vec = vec_inp_vec ^ key_vec;

	res.vec = res_vec;
	for(i=0;i<ELE_COUNT;i++) {
		if(res.data[i] != 0xdeffffff)
			return not_valid();
	}
	return valid();
}
