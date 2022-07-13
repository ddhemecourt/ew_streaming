#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pdw_constructor.h"

void* pdw_constructor(char* pdw_word, struct pdw_s pdw,int inc){
	unsigned long TOA = (pdw.TOA)*(1e-6)*2.4e9;
	bool SEG;
	int freq_offset = (pdw.FREQ_OFFSET)/2.4e9*pow(2,32);
	uint16_t level_offset = pow(10,(-pdw.LEVEL_OFFSET/20))*(pow(2,15)-1);
	uint16_t phase_offset = (pdw.PHASE_OFFSET/360*pow(2,16));
	unsigned long TON = pdw.TON*(1e-6)*2.4e9;
	unsigned int RISE_TIME = (pdw.RISE_TIME)*(1e-9)*2.4e9;
	unsigned int FALL_TIME = (pdw.FALL_TIME)*(1e-9)*2.4e9;

	unsigned long freq_inc;
	unsigned long chip_width;
	unsigned int burst_pri;



	if(pdw.MOD_TYPE == 4){
		SEG = true;
	}
	else{
		SEG = false;
	}
	//header
	pdw_word[0+inc] = (TOA >> 44) & (0xFF);	
	pdw_word[1+inc] = (TOA >> 36) & (0xFF);
	pdw_word[2+inc] = (TOA >> 28) & (0xFF);
	pdw_word[3+inc] = (TOA >> 20) & (0xFF);
	pdw_word[4+inc] = (TOA >> 12) & (0xFF);
	pdw_word[5+inc] = (TOA >> 4) & (0xFF);
	pdw_word[6+inc] = (TOA << 4)|(SEG << 3)|(true << 2)|(0x00);
	
	//flags
	pdw_word[7+inc] = (0x00)|(pdw.M3 << 2)|(pdw.M2 << 1)|(pdw.M1);

	//body
	pdw_word[8+inc] =  (freq_offset >> 24) & (0xFF);
	pdw_word[9+inc] =  (freq_offset >> 16) & (0xFF);
	pdw_word[10+inc] = (freq_offset >> 8) & (0xFF);
	pdw_word[11+inc] = (freq_offset) & (0xFF);
	pdw_word[12+inc] = (level_offset >> 8) & (0xFF);
	pdw_word[13+inc] = (level_offset) & (0xFF);
	pdw_word[14+inc] = (phase_offset >> 8) & (0xFF);
	pdw_word[15+inc] = (phase_offset) & (0xFF);

	
	//payload
	if(pdw.MOD_TYPE == 0){
		pdw_word[16+inc] = (0x00)|(TON >> 40);
		pdw_word[17+inc] = (TON >> 32) & (0xFF);
		pdw_word[18+inc] = (TON >> 24) & (0xFF);
		pdw_word[19+inc] = (TON >> 16) & (0xFF);
		pdw_word[20+inc] = (TON >> 8) & (0xFF);
		pdw_word[21+inc] = (TON) & (0xFF);
		pdw_word[22+inc] = 0x00;
		pdw_word[23+inc] = 0x00;
		pdw_word[24+inc] = 0x00;
		pdw_word[25+inc] = 0x00;
		pdw_word[26+inc] = 0x00;
		pdw_word[27+inc] = 0x00;       	
	}
	else if(pdw.MOD_TYPE == 1){
		pdw_word[16+inc] = (0x01 << 4)|(TON >> 24);
		pdw_word[17+inc] = (TON >> 16) & (0xFF);
		pdw_word[18+inc] = (TON >> 8) & (0xFF);
		pdw_word[19+inc] = (TON) & (0xFF);
		freq_inc = (unsigned long)(pdw.FREQ_INC)/TON/(2.4e9)*pow(2,64);
		//printf("FREQ INC : %ld", freq_inc);
		pdw_word[20+inc] = (freq_inc >> 56) & (0xFF);
		pdw_word[21+inc] = (freq_inc >> 48) & (0xFF);
		pdw_word[22+inc] = (freq_inc >> 40) & (0xFF);
		pdw_word[23+inc] = (freq_inc >> 32) & (0xFF);
		pdw_word[24+inc] = (freq_inc >> 24) & (0xFF);
		pdw_word[25+inc] = (freq_inc >> 16) & (0xFF);
		pdw_word[26+inc] = (freq_inc >> 8) & (0xFF);
		pdw_word[27+inc] = (freq_inc) & (0xFF);       	
	}
	else if(pdw.MOD_TYPE == 2){
		pdw_word[16+inc]= (0x02 << 4)|(TON >> 24);
		pdw_word[17+inc]= (TON >> 16) & (0xFF);
		pdw_word[18+inc]= (TON >> 8) & (0xFF);
		pdw_word[19+inc]= (TON) & (0xFF);
		freq_inc = (pdw.FREQ_INC)/(2.4e9)*pow(2,64);
		pdw_word[20+inc]= (freq_inc >> 56) & (0xFF);
		pdw_word[21+inc]= (freq_inc >> 48) & (0xFF);
		pdw_word[22+inc]= (freq_inc >> 40) & (0xFF);
		pdw_word[23+inc]= (freq_inc >> 32) & (0xFF);
		pdw_word[24+inc]= (freq_inc >> 24) & (0xFF);
		pdw_word[25+inc]= (freq_inc >> 16) & (0xFF);
		pdw_word[26+inc]= (freq_inc >> 8) & (0xFF);
		pdw_word[27+inc]= (freq_inc) & (0xFF);       	
	}
	else if(pdw.MOD_TYPE == 3){
		chip_width = pdw.CHIP_WIDTH*(1e-6)*(2.4e9);
		pdw_word[20+inc] = (0x03 << 4)|(chip_width >> 40);
		pdw_word[21+inc] = (chip_width >> 32) & (0xFF);
		pdw_word[22+inc] = (chip_width >> 24) & (0xFF);
		pdw_word[23+inc] = (chip_width >> 16) & (0xFF);
		pdw_word[24+inc] = (chip_width >> 8) & (0xFF);
		pdw_word[25+inc] = (chip_width) & (0xFF);
		pdw_word[26+inc] = (pdw.CODE << 4)|(0x00);
		pdw_word[27+inc] = 0x00;
		pdw_word[28+inc] = 0x00;
		pdw_word[29+inc] = 0x00;
		pdw_word[30+inc] = 0x00;
		pdw_word[31+inc] = 0x00;      	
	}
	else if(pdw.MOD_TYPE == 4){
		pdw_word[16+inc] = (pdw.SEGMENT_IDX >> 16) & (0xFF); 
		pdw_word[17+inc] = (pdw.SEGMENT_IDX >> 8) & (0xFF);
		pdw_word[18+inc] = (pdw.SEGMENT_IDX) & (0xFF);
		pdw_word[19+inc] = 0x00;
		pdw_word[20+inc] = 0x00;
		pdw_word[21+inc] = 0x00;
		pdw_word[22+inc] = 0x00;
		pdw_word[23+inc] = 0x00;
		pdw_word[24+inc] = 0x00;
		pdw_word[25+inc] = 0x00;
		pdw_word[26+inc] = 0x00;
		pdw_word[27+inc] = 0x00;      	
	}


	//extension flags
	
	if(pdw.BURST == false){
		pdw_word[28+inc] = 0x20;
		pdw_word[29+inc] = 0x00;
	}
	else{
		pdw_word[28+inc] = 0x28;
		pdw_word[29+inc] = 0x00;
	}
	//rise time field
	pdw_word[30+inc] = (pdw.EDGE_TYPE << 5)|(false << 4)|(RISE_TIME >> 18);
	pdw_word[31+inc] = (RISE_TIME >> 10) & (0xFF);
	pdw_word[32+inc] = (RISE_TIME >> 2) & (0xFF);
	pdw_word[33+inc] = (RISE_TIME << 6)|(FALL_TIME >> 16);
        pdw_word[34+inc] = (FALL_TIME >> 8);
	pdw_word[35+inc] = (FALL_TIME);
	//burst field
	burst_pri = (pdw.BURST_PRI)*(1e-6)*(2.4e9);
	pdw_word[36+inc] = (burst_pri >> 24) & (0xFF);
	pdw_word[37+inc] = (burst_pri >> 16) & (0xFF);
	pdw_word[38+inc] = (burst_pri >> 8) & (0xFF);
	pdw_word[39+inc] = (burst_pri) & (0xFF);
	pdw_word[40+inc] = (pdw.BURST_ADD_PULSES >> 8) & (0xFF);
	pdw_word[41+inc] = (pdw.BURST_ADD_PULSES) & (0xFF);
	//unused field
	pdw_word[42+inc] = 0x00;
	pdw_word[43+inc] = 0x00;
	pdw_word[44+inc] = 0x00;
	pdw_word[45+inc] = 0x00;
	pdw_word[46+inc] = 0x00;
	pdw_word[47+inc] = 0x00;

}


void* pdw_inc_toa(char* pdw_word, unsigned long inc){
	uint64_t time;
	uint64_t time_new;
	unsigned char TOA_arr[7] = {pdw_word[0], pdw_word[1], pdw_word[2], pdw_word[3], pdw_word[4], pdw_word[5], pdw_word[6]};
	//printf("\n\n%x  %x  %x  %x  %x  %x  %x", TOA_arr[0], TOA_arr[1], TOA_arr[2], TOA_arr[3], TOA_arr[4], TOA_arr[5], TOA_arr[6]);
	uint64_t x0 = ((0xFF & TOA_arr[5])<<4)|((0xFF & TOA_arr[6])>>4) & 0x000000FF;
	//printf("\n%x\n",x0);
	uint64_t x1 = ((0x000000FF & TOA_arr[4])<<4)|((0x000000FF & TOA_arr[5])>>4) & 0x000000FF;	
	//printf("%x\n",x1);
	uint64_t x2 = ((0xFF & TOA_arr[3])<<4)|((0xFF & TOA_arr[4])>>4) & 0x000000FF;
	//printf("%x\n",x2);
	uint64_t x3 = ((0xFF & TOA_arr[2])<<4)|((0xFF & TOA_arr[3])>>4) & 0x000000FF;
	//printf("%x\n",x3);
	uint64_t x4 = ((0xFF & TOA_arr[1])<<4)|((0xFF & TOA_arr[2])>>4) & 0x000000FF;
	//printf("%x\n",x4);
	uint64_t x5 = ((0xFF & TOA_arr[0])<<4)|((0xFF & TOA_arr[1])>>4) & 0x000000FF;
	//printf("%x\n",x5);
	uint64_t x6 = ((0xFF & TOA_arr[0])>>4) & 0xFF;
	//printf("%x\n",x6);
	time = (x6<<44|x5<<40|x4<<32|x3<<24|x2<<16|x1<<8|x0);
	//time = (x3<<24|x2<<16|x1<<8|x0);
	//printf("\nCurrent TOA: %llx\n",time);
	//time_new = time;
	time_new = time + inc*(1e-6)*2.4e9;
	//printf("Increment: %f\n",inc*(1e-6)*(2.4e9));
	//printf("New TOA: %llx\n",time_new);
	pdw_word[0] = (time_new >> 44) & (0xFF);	
	pdw_word[1] = (time_new >> 36) & (0xFF);
	pdw_word[2] = (time_new >> 28) & (0xFF);
	pdw_word[3] = (time_new >> 20) & (0xFF);
	pdw_word[4] = (time_new >> 12) & (0xFF);
	pdw_word[5] = (time_new >> 4) & (0xFF);
	pdw_word[6] |= (time_new << 4);
}

/*
int main(){

struct pdw_s my_pdw = {0, 0, false, false, false, 0,0,0, false, 0, 20, 0, 0, 0, 0, 0, false, 0, 0};

char *pdw_word = malloc(sizeof(char *)*49);
pdw_constructor(pdw_word,my_pdw);



printf("TEST\n");
for(int i = 0; i<32; i++){
	printf("%x",pdw_word[i]);
}

}
*/
