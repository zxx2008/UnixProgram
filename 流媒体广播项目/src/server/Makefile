CC = gcc
CFLAGS+=-I../include/  -pthread -g
LDFLAGS+=-pthread

SRCS = server.c medialib.c mytbf.c thr_channel.c thr_list.c
OBJS = $(SRCS:.c=.o)

all:server

server:$(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o server -rf