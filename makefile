TARGET:= walking_on_graphs


ALLPROGS= $(TARGET) $(TEST)

.PHONY:	all clean

all: $(ALLPROGS)
progs: $(PROGRAMS)

$(TARGET): 
	$(MAKE) -C src/ ../walking_on_graphs

clean:
	$(MAKE) -C src/ clean
	rm -f $(ALLPROGS)

.PHONY: clean
