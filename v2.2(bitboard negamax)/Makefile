efile = Chess++
ofile = main.o inits.o magic.o moves.o negamax.o

build: $(ofile)
	g++ $(ofile) -o $(efile) -Wall

$@.o: $@.c $@.h
	g++ -c $@.c -Wall

clean:
	rm $(efile) $(ofile) debug_log.txt

run: build
	./Chess++

run_xboard: build
	xboard -debug -nameOfDebugFile debug_log.txt -fcp ./Chess++

test:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 10 -sgf partide.txt -reuseFirst false

testHard:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 3\n" -tc 5 -inc 2 -autoCallFlag true -mg 10 -sgf partide.txt -reuseFirst false

localtest:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "make run" -tc 5 -inc 2 -autoCallFlag true -mg 100 -sgf partide.txt -reuseFirst false
