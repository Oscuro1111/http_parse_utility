GCC=gcc

FLAGS=-fPIC -c -O2

OBJDIR=.

SRCDIR=../src

SRCSFILES=$(wildcard $(SRCDIR)/*.c)

SRCS=$(SRCSFILES:$(SRCDIR)/%.c=%.c)
	
SRC_OBJS=$(patsubst %.c,%.o,$(SRCS))

$(SRC_OBJS):%.o:$(SRCDIR)/%.c
	    $(GCC) $(FLAGS) $^ -o $@

all: $(SRC_OBJS)

build:all
	@echo "build done"
