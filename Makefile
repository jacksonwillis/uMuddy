all: uMuddy.gb

uMuddy.gb: uMuddy.c notes.h
	lcc -o uMuddy.gb uMuddy.c

notes.h: freq_gen
	./freq_gen > notes.h

freq_gen: freq_gen.c
	cc -o freq_gen freq_gen.c

clean:
	rm notes.h freq_gen uMuddy.gb