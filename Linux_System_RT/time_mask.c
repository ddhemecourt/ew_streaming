#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "pdw_constructor.h"
#include "time_mask.h"


static void pdw_sort(struct pdw_s *arr_out, struct pdw_s *arr, int Size[], int num_emitters, int num_pdws){

	int *ptr_val = malloc(sizeof(int)*num_emitters);
       	memset(ptr_val, 0, num_emitters);
	for(int i = 0; i<num_emitters; i++){
	}
	int Time;
	int count = 0;

	struct pdw_s longest = arr[0];
	for(int i = 1; i<num_pdws; i++){
		if(arr[i].TOA > longest.TOA){
			longest = arr[i];
		}
	}

	int move_ptr;
	for(int j = 0; j<num_pdws;j++){ 
		arr_out[j] = longest;
		for(int i = 0; i<num_emitters; i++){
			if(ptr_val[i] < Size[i] && arr[count + ptr_val[i]].TOA < arr_out[j].TOA){
				arr_out[j] = arr[count + ptr_val[i]];
				move_ptr = i;
			}
			count = count + Size[i];
		}

		ptr_val[move_ptr]++;
		count = 0;
	}
}


int time_mask(struct pdw_s pdw, uint64_t *Trise_arr, uint64_t *Tfall_arr, int len){
	for (int i = 0; i<len; i++){
		if(pdw.TOA >= Trise_arr[i] && pdw.TOA <= Tfall_arr[i]){
			return 1;		
		}	
	}
	return 0;
}


struct pdw_s *emitter_to_pdws(struct emitter_s *em, int num_emitters, double us_len, int *num_pdws_out){
	double longest_len = 0;
	struct pdw_s **pdws = (struct pdw_s**)malloc(sizeof(struct pdw_s*) * num_emitters); 
	int *Size = malloc(sizeof(int)*num_emitters);
	struct pdw_s *pdws_out = malloc(sizeof(struct pdw_s)*2048);	

	//find longest PRI
	//for(int j = 0; j<num_emitters; j++){
	//	if(em[j].PRI > longest_len){
	//		longest_len = em[j].PRI;
	//	}
	//}

	longest_len = us_len;

	uint64_t *Trise_arr = malloc(sizeof(uint64_t)*2048);
	uint64_t *Tfall_arr = malloc(sizeof(uint64_t)*2048);
	int pulse_count = 0;
	//Generate PDWs across longest PRI time per input emitter
	for(int j = 0; j<num_emitters; j++){ 
		double num_pdws;
		num_pdws = floor(longest_len/em[j].PRI)+1;
		if(floor(longest_len/em[j].PRI)==(longest_len/em[j].PRI)){num_pdws = num_pdws-1;}
		pdws[j] = malloc(sizeof(struct pdw_s) * num_pdws);
		Size[j] = num_pdws;
		for(int i = 0; i<Size[j]; i++){
			unsigned long TOA = 0 + i*em[j].PRI;
			//struct pdw_s pdw = {5000000, 0, false, false, true, 50000000, 0,0, false, 0, 20, 0, 0, 0, 0, 0, false, 0, 0};	
			struct pdw_s pdw = {TOA, 0, false, false, false, 0,0,0, false, 0, em[j].PW, 0, 0, 0, 0, 0, false, 0, 0};
			pdws[j][i] = pdw;
			Trise_arr[pulse_count] = pdw.TOA;
			Tfall_arr[pulse_count] = pdw.TOA + pdw.TON;
			pulse_count++;
		}
	}
	int mask_cnt = 0;
	int count = 0;
	int new_size = 0;
	for(int j = 0; j < num_emitters; j++){
		for(int i = 0; i<Size[j]; i++){
			if(j == 0){
				pdws_out[count] = pdws[j][i];
				count++;
				new_size++;
			}
			else if(time_mask(pdws[j][i],Trise_arr,Tfall_arr,mask_cnt) == 0)
			{
				pdws_out[count] = pdws[j][i];
				count++;
				new_size++;
			}
		}
		mask_cnt = mask_cnt + Size[j];
		Size[j] = new_size;
		new_size = 0;
	}

	struct pdw_s *pdw_out = malloc(sizeof(struct pdw_s)*count);
	pdw_sort(pdw_out,pdws_out,Size,num_emitters,count);
	*num_pdws_out = count;
	return pdw_out;
}

//int main(){
//
//	struct emitter_s em_arr[3]; 
//	em_arr[0] = (struct emitter_s){0, 50, 500};
//	em_arr[1] = (struct emitter_s){0, 5, 40};
//	em_arr[2] = (struct emitter_s){0, 10, 90};
//	int num_pdws_out;
//	struct pdw_s *pdw_out = emitter_to_pdws(em_arr, 3,1000, &num_pdws_out);
//	for(int i = 0; i<(num_pdws_out); i++){
//		printf("%lu\t %lu\n", pdw_out[i].TOA,pdw_out[i].TON);
//	}
//	printf("\n");
//	return 0;
//
//}




