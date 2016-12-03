all: Addressbook Server

Addressbook: main.c
	gcc -Wall -o Addressbook main.c `pkg-config gtk+-3.0 --cflags --libs` -lsocketcomm

Server: server.c
	gcc -Wall -o Server server.c -lsocketcomm
	
clean:
	rm Addressbook Server
