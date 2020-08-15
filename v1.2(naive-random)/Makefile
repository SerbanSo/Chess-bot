efile = Chess++
ofile = main.o inits.o moves.o

build: $(ofile)
	g++ $(ofile) -o $(efile)

$@.o: $@.c $@.h
	g++ -c $@.c

clean:
	rm $(efile) $(ofile) debug_log.txt

run: build
	xboard -debug -nameOfDebugFile debug_log.txt -fcp ./Chess++
