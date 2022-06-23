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

};


void process_pdw_file(char *filename, struct emitter_s *em_arr, int *num_em);

void process_input_file(char *filename, struct emitter_ptr_s *em_arr, int *num_em_ptrs);
