CC = gcc

OBJDIR = obj
SRCDIR = src
BINDIR = bin
INCLUDEDIR = include
LIBDIR = lib
TESTDIR = test

CFLAGS = -std=gnu99 -g -O3 -Wall -I$(INCLUDEDIR)
LFLAGS = -lm -pthread
ifeq ($(INCLUDE_SNAPSHOTS),"TRUE")
      CFLAGS += -DENABLE_SNAPSHOTS
endif

INCLUDE = $(INCLUDEDIR)/*.h

OBJ = $(OBJDIR)/arguments.o \
	  $(OBJDIR)/buffer.o \
	  $(OBJDIR)/cdn.o \
	  $(OBJDIR)/driver.o \
	  $(OBJDIR)/edge.o \
	  $(OBJDIR)/flow.o \
	  $(OBJDIR)/link.o \
	  $(OBJDIR)/links.o \
	  $(OBJDIR)/memory.o \
	  $(OBJDIR)/packet.o \
	  $(OBJDIR)/transmitter.o \
	  $(OBJDIR)/user.o

default: all

# list of all the targets to be generated
all: $(BINDIR)/driver

# % is the wildcard, $< means the first item to the right of :
$(OBJDIR)/%.o: $(LIBDIR)/%.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS)

# $@ means everything to the left of :, $^ means everything to the right of :
$(BINDIR)/%: $(OBJDIR)/%.o $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

# to test modules
test: $(BINDIR)/test

# % is the wildcard, $< means the first item to the right of :
$(OBJDIR)/%.o: $(LIBDIR)/%.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(TESTDIR)/%.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS)

# $@ means everything to the left of :, $^ means everything to the right of :
$(BINDIR)/%: $(OBJDIR)/%.o $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

# to ensure make doesnot delete intermediate files like *.o
.PRECIOUS: $(OBJDIR)/%.o

.PHONY: default all clean

clean:
	rm -f $(OBJDIR)/*.o; rm -f $(BINDIR)/*;