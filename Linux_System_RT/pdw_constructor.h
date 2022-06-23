#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

struct pdw_s {

	uint64_t TOA : 52;
	unsigned int MOD_TYPE : 4;
	bool M3;
	bool M2;
	bool M1;
	int FREQ_OFFSET;
	float LEVEL_OFFSET;
	float PHASE_OFFSET;
	bool EDGE_TYPE;
	unsigned int SEGMENT_IDX : 24;
	unsigned long TON;
	long FREQ_INC;
	unsigned long CHIP_WIDTH : 44;
	unsigned int CODE : 4;
	unsigned int RISE_TIME : 22;
	unsigned int FALL_TIME : 22;
	bool BURST;
	unsigned int BURST_PRI;
	uint16_t BURST_ADD_PULSES;
	

}; 

//pdw_default = {0, 0, false, false, false, 0,0,0, false, 0, 20, 0, 0, 0, 0, 0, false, 0, 0};

void* pdw_constructor(char* pdw_word, struct pdw_s pdw, int inc);


void* pdw_inc_toa(char* pdw_word, unsigned long inc);
