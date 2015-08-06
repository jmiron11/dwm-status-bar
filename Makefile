CC = gcc
CFLAGS = -Wall -pedantic
LFLAGS = -lX11 -lasound

TARGET = dwmstatus

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(LFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
