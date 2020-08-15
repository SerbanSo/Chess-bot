efile = Chess++
ofile = main.o inits.o moves.o negamax.o

build: $(ofile)
	g++ -O2 $(ofile) -o $(efile)

$@.o: $@.c $@.h
	g++ -c $@.c

clean:
	rm $(efile) $(ofile) debug_log.txt

run: build
	./Chess++

runxboard: build
	xboard -debug -nameOfDebugFile debug_log.txt -fcp ./Chess++

test:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 10 -sgf partide.txt -reuseFirst false
