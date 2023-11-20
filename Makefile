CFLAGS = -O3

all: lab.o
	g++ $(CFLAGS) autograder.o lab.o -o lab

lab.o: lab.cc
	g++ $(CFLAGS) -c lab.cc -o lab.o

debug:
	g++ -g -c lab.cc -o lab.o
	g++ -g autograder.o lab.o -o lab

clean:
	rm lab.o lab