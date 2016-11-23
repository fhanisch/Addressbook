all:
	gcc -g -o Addressbook main.c `pkg-config gtk+-3.0 --cflags --libs`

