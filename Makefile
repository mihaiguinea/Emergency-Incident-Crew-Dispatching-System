CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = tema1
SRC = tema1.c

build: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET) tema1.out

.PHONY: build run clean
