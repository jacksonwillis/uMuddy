#include <stdio.h>
#include <gb/gb.h>
#include "notes.h"

#define UMUDDY_VERSION "0.0.1"

#define PRESSED(KEY) keys & J_## KEY
#define SOUND_ON() NR12_REG=0xF0
#define SOUND_OFF() NR12_REG=0
#define SOUND_INIT() NR50_REG=0xFF;NR51_REG=0xFF;NR52_REG=0x80;
#define PULSE_12_5 // UNKNOWN
#define PULSE_25 0x20
#define PULSE_50 0xA0
#define PULSE_WIDTH(N) NR11_REG=PULSE_## N
#define NOTE_DELAY 50

const char *scale[] = { ".","C3","D3","E3","F3","G3","A3","B3","C4" };

int octave=0;
int pulse_mode=0;

void play_freq(int f) { NR13_REG=(unsigned char)f;NR14_REG=0x80|(f>>8); }
void play_note(int n) { play_freq(note_frequencies[n - 1]); }

int scale_position(UBYTE keys) {
	if (PRESSED(LEFT)) {
		if (PRESSED(UP)) {
			return 8;
		} else if (PRESSED(DOWN)) {
			return 2;
		} else {
			return 1;
		}
	} else if (PRESSED(DOWN)) {
		if (PRESSED(RIGHT)) {
			return 4;
		} else {
			return 3;
		}
	} else if (PRESSED(RIGHT)) {
		if (PRESSED(UP)) {
			return 6;
		} else {
			return 5;
		}
	} else if (PRESSED(UP)) {
		return 7;
	} else {
		return 0;
	}
}

void select_menu() {
	pulse_mode = (pulse_mode + 1) % 2; // 3;

	printf("\n* pulse width ");

	switch (pulse_mode) {
	case 0:
		printf("50");
		PULSE_WIDTH(50);
		break;
	case 1:
		printf("25");
		PULSE_WIDTH(25);
		break;
	/*
	case 2:
		printf("12.5");
		PULSE_WIDTH(12_5);
		break;
	*/
	}

	printf(".\n");
	delay(500);
}

void main() {
	int this_note, last_note = 0;
	UBYTE keys;
	
	printf("    uMuddy %s\n\n", UMUDDY_VERSION);
	printf("github.com/\n   jacksonwillis/\n              uMuddy\n");
	
	SOUND_INIT();
	
	while (1) {
		keys = joypad();
		
		if (PRESSED(START)) {
			octave = !octave;
			printf("\n* octave +%d\n", octave);
			waitpadup();
		} else if (PRESSED(SELECT)) {
			select_menu();
		}
		
		this_note = scale_position(keys);

		if (this_note != last_note) {
			printf("%s ", scale[this_note]);
		
			if (this_note != 0) {
				if (this_note != last_note) SOUND_ON();
				play_note(octave ? this_note + 7 : this_note);	
			} else {
				SOUND_OFF();
			}
		}

		last_note = this_note;
		delay(NOTE_DELAY);
	}
}