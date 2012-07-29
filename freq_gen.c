#include <stdio.h>
static float notes[] = {
	65.41, // C2
	73.42, // D2
	82.41, // E2
	87.31, // F2
	98.00, // G2
	110.0, // A2
	123.5, // B2
	130.8, // C3 
	146.8, // D3
	164.8, // E3
	174.6, // F3
	196.0, // G3
	220.0, // A3
	246.9, // B3
	261.6  // C4
};
int frequency(float hertz) { return (2048 * (hertz - 64)) / hertz; }
int main() {
	printf("const int note_frequencies[]={");
	for (int i = 0; i < sizeof(notes)/sizeof(float); i += 1) {
		printf("%d,", frequency(notes[i]));
	}
	printf("};");
	return 0;
}