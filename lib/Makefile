CC=gcc

CFLAGS=-DHAVE_CONFIG_H -g -O2
INCLUDES=-I. -I.. -I./.. -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include

OBJS = list.o \
					 neuralnet.o \
					 mt19937ar.o \
					 isaac.o \
					 md5.o \
					 cache.o \
					 inputs.o


%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<

libs: $(OBJS) 
	ar rcs libevent.a $(OBJS)

clean:
	rm -rf $(OBJS) libevent.a
