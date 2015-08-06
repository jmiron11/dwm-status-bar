CC = gcc
CFLAGS = -Wall -pedantic
LFLAGS = -lX11 -lasound

TARGET = dwmstatus

all: dwmstatus

dwmstatus: dwmstatus.o getstatus.o
	$(CC) $(CFLAGS) $(LFLAGS) -o dwmstatus dwmstatus.o getstatus.o

dwmstatus.o: src/dwmstatus.c src/getstatus.h
	$(CC) $(CFLAGS) -c src/dwmstatus.c

getstatus.o: src/getstatus.c src/getstatus.h
	$(CC) $(CFLAGS) -c src/getstatus.c

clean:
	$(RM) dwmstatus *.o
