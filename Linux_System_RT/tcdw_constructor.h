
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

struct tcdw_s {

	unsigned long TOA : 52;
	bool PATH; //false = Path A, true = Path B
	unsigned int CMD : 3; //0 = freq, 1 = amp, 2 = freq and amp, 3 = rearm 
	unsigned long FVAL : 40;
	bool L_POSNEG;
	unsigned int L_INT : 7;
	unsigned int L_TENTH : 4;
	unsigned int L_HUND : 4;

};

static struct tcdw_s tcdw_default = {5001000, false, 2, 12000000000, true, 10, 0, 0};

void* tcdw_constructor(char* tcdw_word, struct tcdw_s tcdw);


