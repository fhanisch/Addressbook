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
	status st;
	int connected;
	
	if(argc>1)
	{
		if(!strcmp(argv[1],"-v"))
		{
			printf("*** Adressbuch Server ***   Author: FH   Version 0.1   24.11.2016\n");
			return ok;
		}
		else
		{
			printf("Unknown command!\n");
			return ok;
		}
	}
	
	printf("Addressbook Server\n");
	
	st = createServer(port);
	if (st)
	{
			printf("%s",getLastErr());
			return err1;
	}			
	
	while(1)
	{
		printf("Warte auf Verbindungen...\n");
		
		st = waitForConnections();
		if (st)
		{
			printf("%s",getLastErr());
			return err1;
		}		
		connected = 1;
		
		while(connected)
		{
			st = rcvCmdFromClient();
			if(st==noConn)
			{		
				printf("%s",getLastErr());		
				connected=0;				
			}			
		}
	}
	
	closeServer();	
	
	return ok;
}
