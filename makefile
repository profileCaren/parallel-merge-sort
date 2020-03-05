CC = g++

CPFLAGS = -DHOMEGROWN
LPFLAGS = -pthread

# CFLAGS = -mcx16 -O3 -std=c++11 -march=native -Wall $(CPFLAGS)
CFLAGS = -mcx16 -std=c++17 -march=native -Wall $(CPFLAGS) --debug

all:	scan main

scan: scan.cpp scan.h
	$(CC) $(CFLAGS) scan.cpp -o out/scan.out $(LPFLAGS)

main: main.cpp
	$(CC) $(CFLAGS) main.cpp -o out/main.out $(LPFLAGS)

clean:
	rm -f scan
