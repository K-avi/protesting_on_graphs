TARGET := ../template_name

TEST := ../test
CC := gcc
CFLAGS := -O2  -g -Wall -Wextra -Wpedantic -Wno-unused-parameter

SRCS := graph.c memory.c rules.c trace.c walker.c 

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(CLIBS)


$(TEST): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $^ -o $(TEST) $(CLIBS)


clean:
	rm -f parser.tab.* parser.output lex.*.*  
	
.PHONY: clean
