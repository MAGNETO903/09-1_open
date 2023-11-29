CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -fPIC
LD = gcc
LDFLAGS = -ldl -rdynamic

.PHONY: all clean

all: printrandom

printrandom: main.o linear.so random.so
	$(CC) $(CFLAGS) -fPIC -o $@ $^ $(LDFLAGS) -Wl,-rpath,.

main.o: main.c random_source.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

linear.so: linear.o random_source.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

linear.o: linear.c random_source.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

random.so: random.o random_source.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

random.o: random.c random_source.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	rm -f printrandom main.o linear.so linear.o random.so random.o
