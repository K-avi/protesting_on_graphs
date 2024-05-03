# protesting_on_graphs: A simulation of .... protesters on graphs
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

TARGET:= protesting_on_graphs


ALLPROGS= $(TARGET) $(TEST)

.PHONY:	all clean

all: $(ALLPROGS)
progs: $(PROGRAMS)

$(TARGET): 
	$(MAKE) -C src/ ../protesting_on_graphs

clean:
	$(MAKE) -C src/ clean
	rm -f $(ALLPROGS)

.PHONY: clean
