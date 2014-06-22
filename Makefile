CC=g++
CFLAGS=-W -Wall -Wextra -pedantic -DNDEBUG -O2
LDFLAGS=
PROJ=ahed

all: $(PROJ)

$(PROJ): main.o io.o ahed.o tree.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cc ahed.h
	$(CC) $(CFLAGS) -c -o $@ $<

io.o: io.cc io.h
	$(CC) $(CFLAGS) -c -o $@ $<

ahed.o: ahed.cc ahed.h io.h tree.h
	$(CC) $(CFLAGS) -c -o $@ $<

tree.o: tree.cc tree.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(PROJ)

