/*
__________________________________________________________________________________

	gcc -o Server server.c
__________________________________________________________________________________
*/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int srcv(int sock, char *buf)
{
  int numbytes;
  
  numbytes=recv(sock,buf,32,0);
  if (numbytes<=0)
  {
    printf("Empfangen fehlgeschlagen!\n");    
    return 1;
  }  
  printf("%i Bytes empfangen: %s\n",numbytes,buf);
  return 0;
}

int main(int argc, char **argv)
{
	int acceptsocket, serversocket;
	struct sockaddr_in addr, client_addr;
	int sz_sock = sizeof(struct sockaddr_in);
	int port=12345;
	int numbytes;
	char rcvbuf[256];
	int status;
	int connected;
	
	if(argc>1)
	{
		if(!strcmp(argv[1],"-v"))
		{
			printf("*** Adressbuch Server ***   Author: FH   Version 0.1   24.11.2016\n");
			return 0;
		}
		else
		{
			printf("Unknown command!\n");
			return 0;
		}
	}
	
	printf("Addressbook Server\n");
	acceptsocket=socket(AF_INET,SOCK_STREAM,0);	
	if(acceptsocket<0)
	{
		printf("Fehler: Der Socket konnte nicht erstellt werden!\n");
		return -1;
	}
	printf("Accept Socket erstellt: %d\n",acceptsocket);
	
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=INADDR_ANY;
	
	if (bind(acceptsocket,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		printf("Bind fehlgeschlagen!\n");
		close(acceptsocket);
		return -1;
	}
	printf("Bind Socket mit Port: %i\n\n",port);
	
	if (listen(acceptsocket,10)<0)
	{
		printf("Listen fehlgeschlagen!\n");
		close(acceptsocket);
		return -1;
	}
	
	while(1)
	{
		printf("Warte auf Verbindungen...\n");
		
		serversocket=accept(acceptsocket,(struct sockaddr*)&client_addr,(socklen_t*)&sz_sock);
		if(serversocket<0)
		{
			printf("Verbindung fehlgeschlagen!\n");
			close(acceptsocket);
			return -1;
		}
		printf("Verbindung akzeptiert. Verbunden mit Serversocket: %d\n",serversocket);
		connected = 1;
		while(connected)
		{
			status = srcv(serversocket,rcvbuf);
			if(status)
			{
				printf("Verbindung unterbrochen!\n");
				connected=0;
				close(serversocket);
			}
			else
			{
				
			}
		}
	}
	close(serversocket);
	close(acceptsocket);	
	return 0;
}
