CC?=gcc
#CFLAGS?=-g -I/usr/src/kernels/2.6.32-431.el6.x86_64/include
#LDFLAGS?=-L/usr/local/libev-4.22/lib
LIBS?=-lpthread

all:server
.PHONY:all

target = server
objects = main.o server_socket.o tunable.o


$(target):$(objects)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(objects):%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY:clean
	
clean:
	rm -rf *.o $(target) $(target)

