enigma : main.o coders.o enigma.o
	g++ -g -O0 -Wall main.o coders.o enigma.o -o enigma

main.o: main.cpp coders.h errors.h enigma.h
	g++ -g -O0 -Wall -c main.cpp

coders.o : coders.cpp coders.h errors.h enigma.h
	g++ -g -O0 -Wall -c coders.cpp

enigma.o : enigma.cpp enigma.h coders.h
	g++ -g -O0 -Wall -c enigma.cpp

clean:
	-rm -f *.o main
