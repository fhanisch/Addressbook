all:
	gcc -o Adressbook main.c `pkg-config gtk+-3.0 --cflags --libs`

