TARGET := ../walking_on_graphs
CC := gcc
CFLAGS := -O1    -g -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=gnu17

SRCS := graph_walkers/*.c movement/*.c tactics/*.c common.c misc.c main.c 

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(CLIBS)

 
	
.PHONY: clean
