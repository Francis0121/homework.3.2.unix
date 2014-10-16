CC = gcc
LD = gcc

INCDIR = ../../command/include
SRCDIR1 = ../../command/util
SRCDIR2 = ../module

VPATH = $(SRCDIR1) $(SRCDIR2) ..

CFLAGS = -O2 -I$(INCDIR)

TARGET = sh

SRCS = $(foreach dir, .. $(SRCDIR1) $(SRCDIR2), $(wildcard $(dir)/*.c))
SRCS := $(notdir $(SRCS))

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(LD) $^ -o$(TARGET) $(LIBS)
	mv $(TARGET) ../


%o:%c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(OBJS)
	-rm -f ../$(TARGET)

depend: $(SRCS)
	$(CC) -M $(CFLAGS) $^ > $@

-include depend
