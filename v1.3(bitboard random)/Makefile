efile = Chess++
ofile = main.o magic.o inits.o moves.o 

build: $(ofile)
	g++ $(ofile) -o $(efile) -Wall

$@.o: $@.c $@.h
	g++ -c $@.c -Wall 

clean:
	rm $(efile) $(ofile) bit_board.txt board.txt debug_log.txt input_from_xboard.txt

run: build
	./Chess++

run_test: build
	xboard -debug -nameOfDebugFile debug_log.txt -fcp ./Chess++

test:
	xboard -debug -nameOfDebugFile debug_log.txt -fcp "make run" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 100 -sgf partide.txt -reuseFirst false

