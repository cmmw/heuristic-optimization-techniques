SRCDIR=src

.PHONY: all $(SRCDIR)

default: all

all: $(SRCDIR)

$(SRCDIR): 
	${MAKE} -C $(SRCDIR) all
	
clean:
	${MAKE} -C $(SRCDIR) clean