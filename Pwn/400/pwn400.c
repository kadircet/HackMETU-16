#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

void handle_client(int client)
{
	for(int i=0;i<3;i++)
		close(i), dup(client);
	int name=0;
	char inp[128];
	fread(inp, 1, 12, stdin);
	printf(inp);
	fflush(stdout);
	fread(inp, 1, 128, stdin);
	printf(inp);
	if(name == 0x01071514)
		puts("FLAG");
	fflush(stdout);
	shutdown(client, 2);
	char a[20];
	while(recv(client, a, sizeof(a), 0)>0);
	close(client);
	exit(0);
}

int running=1;
void sigterm(int x)
{
	__sync_fetch_and_sub(&running, 1);
}

int main(int argc, const char *argv[])
{
	int sck, client, addrlen;
	struct sockaddr_in this_addr, peer_addr;
	pid_t child_pid;
	unsigned short port = 1337;
	
	addrlen = sizeof( struct sockaddr_in );
	memset( &this_addr, 0, addrlen );
	memset( &peer_addr, 0, addrlen );
	
	this_addr.sin_port        = htons(port);
	this_addr.sin_family      = AF_INET;
	this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	sck = socket( AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(sck==-1)
		puts("sck");
	if(bind(sck, (struct sockaddr *)&this_addr, addrlen)!=0)
		puts("bind");
	if(listen(sck, 5)!=0)
		puts("listen");
	
	signal(SIGINT, sigterm);
	while(running)
	{
		while(running && -1 == (client = accept(sck, (struct sockaddr *)&peer_addr, &addrlen)));
		if(running)
		{
			printf("Connection from: %d.%d.%d.%d\n",
				(int)(peer_addr.sin_addr.s_addr&0xFF),
				(int)((peer_addr.sin_addr.s_addr&0xFF00)>>8),
				(int)((peer_addr.sin_addr.s_addr&0xFF0000)>>16),
				(int)((peer_addr.sin_addr.s_addr&0xFF000000)>>24));
		
			if(!fork())
				handle_client(client);
		}
	}

	close(sck);
	return 0;
}
