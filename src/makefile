TARGET := ../walking_on_graph
TEST := ../test
CC := gcc
CFLAGS := -O2  -g -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=gnu17

SRCS := misc.c tactics.c walker.c  graph_table.c movement.c

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(CLIBS)


$(TEST): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $^ -o $(TEST) $(CLIBS)
 
	
.PHONY: clean
