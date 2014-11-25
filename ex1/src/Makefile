CCFLAGS = -g -pedantic -Wall
CC = g++
NAME = tcbvrp_solver

HEADERS := $(wildcard *.h)

SRCS := $(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

DEPS=$(SRCS:.cpp=.d)


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CCFLAGS) -o $(NAME) $^
	mv $(NAME) ..
	

.SUFFIXES: .o .cpp .d
.cpp.o:
	$(CC) -c $(CCFLAGS) -o $@ $<

.cpp.d:
	$(CC) -MM $(CCFLAGS) -MF $@ $<
		
clean:
	rm -f $(OBJS) $(DEPS) ../$(NAME)
	
-include $(DEPS)