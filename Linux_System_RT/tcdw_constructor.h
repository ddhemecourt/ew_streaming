
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

struct tcdw_s {

	unsigned long TOA : 52;
	bool PATH; //false = Path A, true = Path B
	char *CMD; //0 = freq, 1 = amp, 2 = freq and amp, 3 = rearm 
	unsigned long FVAL : 40;
	float LVAL;
};

static struct tcdw_s tcdw_default = {5001000, false, "amp", 12000000000, -7.52};

void* tcdw_constructor(char* tcdw_word, struct tcdw_s tcdw);


