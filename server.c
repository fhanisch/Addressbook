/*
__________________________________________________________________________________

	gcc -o Server server.c
__________________________________________________________________________________
*/

#include <stdio.h>
#include <string.h>
#include <socket_comm.h>


int main(int argc, char **argv)
{
	int port=12345;	
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
	
	status = createServer(port);
	if (status)
	{
			printf("%s",getLastErr());
			return -1;
	}			
	
	while(1)
	{
		printf("Warte auf Verbindungen...\n");
		
		status = waitForConnections();
		if (status)
		{
			printf("%s",getLastErr());
			return -1;
		}		
		connected = 1;
		
		while(connected)
		{
			status = rcvCmdFromClient();
			if(status)
			{		
				printf("%s",getLastErr());		
				connected=0;				
			}
			else
			{
				
			}
		}
	}
	
	closeServer();	
	
	return 0;
}
