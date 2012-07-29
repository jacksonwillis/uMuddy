all: uMuddy.gb

uMuddy.gb: uMuddy.c notes.h
	lcc -o uMuddy.gb uMuddy.c

clean:
	rm notes.h freq_gen uMuddy.gb