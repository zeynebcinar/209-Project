# Makefile for Dungeon Adventure Game

CC = gcc
CFLAGS = -Wall -g
TARGET = dungeon_game
SRCS = dungeon_game.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
