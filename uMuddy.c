#include <stdio.h>
#include <gb/gb.h>

#define UMUDDY_VERSION "0.0.1"

#define PULSE_12_5 // UNKNOWN
#define PULSE_25 0x20
#define PULSE_50 0xA0
#define PULSE_WIDTH(N) NR11_REG=PULSE_## N

const int note_frequencies[] = {44,262,457,546,710,856,986,1045,1155,1252,1297,1379,1452,1517,1546};
const char *note_names[] = {" . ","C 3","D 3","E 3","F 3","G 3","A 3","B 3","C 4","D 4","E 4","F 4","G 4","A 4","B 4","C 5","D 5","E 5","F 5","G 5","A 5","B 5"};

// we're only using channel one.
void sound_on() { NR12_REG = 0xFF; } // volume all the way up
void sound_off() { NR12_REG = 0; } // volume set to 0
void sound_init() { NR50_REG = 0xFF; NR51_REG = 0xFF; NR52_REG = 0x80; }
void play_freq(int f) { NR13_REG = (unsigned char)f; NR14_REG = 0x80|(f>>8); }
void play_note(int n) { play_freq(note_frequencies[n - 1]); }

/*
input: data from joypad();
returns: integer between 0 and 8.
if no keys on d-pad are pressed, it returns 0.
else, returns number corresponding to this diagram:

      7
  8   __  6
   __|  |__
1 |__    __| 5
   2 |__| 4
      3
*/
#define PRESSED(KEY) keys & J_## KEY
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

int pulse_mode = 0;
int octave = 0; // is note raised an octave?

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
	int menu_mode, this_note, last_note = 0;
	UBYTE keys;
	
	printf("    uMuddy %s\n\n", UMUDDY_VERSION);
	printf("github.com/\n   jacksonwillis/\n              uMuddy\n");
	
	sound_init();
	
	while (1) {
		keys = joypad();
		
		if (PRESSED(START)) {
			octave = !octave;
			printf("\n* octave +%d\n", octave);
			waitpadup();
		} else if (PRESSED(SELECT)) {
			menu_mode = 1;
			select_menu();
		}
		
		this_note = scale_position(keys);
		
		if (this_note && octave) this_note += 7;
		if (this_note && PRESSED(A)) this_note += 4;

		if (this_note != last_note) {
			printf("%s ", note_names[this_note]);
		
			if (this_note) {
				if (this_note != last_note) sound_on();
				play_note(this_note);	
			} else {
				sound_off();
			}
		}

		last_note = this_note;
		
		// wait for next input, accuracy of 50ms
		delay(50);
	}
}