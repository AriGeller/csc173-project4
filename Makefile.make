PROGRAMS = main

CFLAGS = -g

programs: $(PROGRAMS)

main: Database.o

clean:
	-rm $(PROGRAMS) *.o
	-rm -r *.dSYM
