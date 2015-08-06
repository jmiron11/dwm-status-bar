CC = gcc
CFLAGS = -Wall -pedantic
LFLAGS = -lX11 -lasound

TARGET = dwmstatus

all: dwmstatus

dwmstatus: dwmstatus.o getstatus.o
	$(CC) $(CFLAGS) $(LFLAGS) -o dwmstatus dwmstatus.o getstatus.o

dwmstatus.o: dwmstatus.c getstatus.h
	$(CC) $(CFLAGS) -c dwmstatus.c

getstatus.o: getstatus.c getstatus.h
	$(CC) $(CFLAGS) -c getstatus.c

clean:
	$(RM) dwmstatus *.o
