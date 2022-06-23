#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "time_mask.h"

struct emitter_ptr_s {

	int timestep;
	char *type;
	char *filename;
	int *basebands;
	int BB_num;
	
	//TCDW info
	bool PATH; //false = Path A, true = Path B
	char *CMD; //0 = freq, 1 = amp, 2 = freq and amp, 3 = rearm 
	unsigned long FVAL : 40;
	float LVAL;

};


void process_pdw_file(char *filename, struct emitter_s *em_arr, int *num_em);

void process_input_file(char *filename, struct emitter_ptr_s *em_arr, int *num_em_ptrs);
