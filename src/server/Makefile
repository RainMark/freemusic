CC ?= gcc
CFLAGS += -lmysqlclient -I../include

SRC = server.c sql.c
OBJS = server.o sql.o

all: fmd

fmd: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(@F)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $(@F)

clean:
	rm *.o fmd
