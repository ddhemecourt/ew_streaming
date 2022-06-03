#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

struct adw_s {

	bool SEG;
	bool USE_EXTENSION;
	bool SEG_INTERRUPT;
	bool IGNORE_ADW;
	bool M3;
	bool M2;
	bool M1;
	int FREQ_OFFSET;
	uint16_t  LEVEL_OFFSET;
	uint16_t  PHASE_OFFSET;
	unsigned int SEGMENT_IDX : 24;
	unsigned int BURST_SRI;
	uint16_t BURST_ADD_SEGMENTS;
}; 

//adw_default = {true, false, false, false, false, false, false, 0, 0, 0, 0, 0, 0};

void* adw_constructor(char* adw_word,struct adw_s adw);
