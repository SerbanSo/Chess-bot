efile = Chess++
ofile = main.o inits.o magic.o moves.o negamax.o eval.o

build: $(ofile)
	g++ $(ofile) -o $(efile) -Wall -std=c++11

$@.o: $@.c $@.h
	g++ -c $@.c -Wall

clean:
	rm $(efile) $(ofile) 

run: 
	./Chess++

run_xboard: build
	xboard -debug -nameOfDebugFile debug_log.txt -fcp ./Chess++

testEasy:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false

test1:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 3\n" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false

test2:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 4\n" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false

localtest:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "make run" -tc 5 -inc 2 -autoCallFlag true -mg 100 -sgf partide.txt -reuseFirst false
