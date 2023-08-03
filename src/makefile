# walking_on_graphs: A simulation of .... walkers on graphs
# Copyright (C) 2023 Ivan Mulot-Radojcic 
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 or later.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
# for more details: https://www.gnu.org/licenses/

TARGET := ../walking_on_graphs
CC := gcc
CFLAGS := -O1    -g -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=gnu17

SRCS := graph_walkers/*.c movement/*.c tactics/*.c common.c misc.c 

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(CLIBS)

 
	
.PHONY: clean
