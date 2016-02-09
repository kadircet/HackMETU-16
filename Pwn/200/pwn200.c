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

typedef struct _node
{
	char *msg;
	struct _node *nxt,*prv;
	void *padder;
} node;

void handle_client(int client)
{
	for(int i=0;i<3;i++)
		close(i), dup(client);

	node *head=NULL,*nnode;
	char *tmp;
	int cmd;
	char *pass = "FLAG";
	puts("Welcome to message logger:");
	puts("0: enter a log, which must be 32 chars in length");
	puts("1 x: delete entry x");
	puts("2: list entries");
	while(!feof(stdin))
	{
		fflush(stdout);
		scanf("%d", &cmd);
		switch(cmd)
		{
			case 0:
				tmp=malloc(32);
				fread(tmp, 1, 32, stdin);
				fflush(stdout);
				nnode=malloc(sizeof(node));
				nnode->msg=tmp;
				nnode->nxt=head;
				nnode->prv=NULL;
				if(head)
					head->prv=nnode;
				head=nnode;
				break;
			case 1:
				scanf("%d", &cmd);
				nnode=head;
				for(int i=0;i<cmd&&nnode!=NULL;i++)
					nnode=nnode->nxt;
				if(nnode==NULL)
					puts("out of range??");
				else
				{
					if(nnode->prv)
						nnode->prv->nxt=nnode->nxt;
					if(nnode->nxt)
						nnode->nxt->prv=nnode->prv;
					free(nnode);
				}
				break;
			case 2:
				nnode=head;
				for(int i=0;nnode&&i<3;i++)
				{
					printf("%d : %s\n", i, nnode->msg);
					fflush(stdout);
					nnode=nnode->nxt;
				}
				break;
		}
	}
	
	fflush(stdout);
	shutdown(client, 2);
	char passed[100];
	while(recv(client, passed, sizeof(passed), 0)>0);
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

