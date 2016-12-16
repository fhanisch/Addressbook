EXEC = Addressbook

all: $(EXEC) Server

Addressbook: main.c
	gcc -Wall -o $(EXEC) main.c `pkg-config gtk+-3.0 --cflags --libs` -lsocketcomm

Server: server.c
	gcc -Wall -o Server server.c -lsocketcomm
	
clean:
	rm $(EXEC) Server
