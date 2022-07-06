#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "tcdw_constructor.h"

void* tcdw_constructor(char* tcdw_word,struct tcdw_s tcdw){
	
	unsigned long TOA = (tcdw.TOA)*(1e-6)*2.4e9;
	bool L_POSNEG;
	unsigned int CMD;
	unsigned int L_INT;
	unsigned int L_TENTH;
	unsigned int L_HUND;

	if(tcdw.LVAL <= 0){
		L_POSNEG = true;
	}
	else{
		L_POSNEG = false;
	}

	float temp;
	if(tcdw.LVAL < 0){
		temp = -1*tcdw.LVAL;
	}
	else{
		temp = tcdw.LVAL;
	}
	

	L_INT = (unsigned int)floor(temp);
	L_TENTH = (unsigned int)floor(10*(temp-floor(temp)));
	L_HUND = floor(100*(temp-floor(temp)))-10*(floor(10*(temp-floor(temp))));

	if(!strcmp("freq",tcdw.CMD)){
		CMD = 0;
	}	
	else if(!strcmp("amp",tcdw.CMD)){
		CMD = 1;
	}
	else if(!strcmp("freq_amp",tcdw.CMD)){
		CMD = 2;
	}
	else if(!strcmp("rearm",tcdw.CMD)){
		CMD = 3;
	}
	

	tcdw_word[0] =  (TOA >> 44) & (0xFF);	
	tcdw_word[1] =  (TOA >> 36) & (0xFF);
	tcdw_word[2] =  (TOA >> 28) & (0xFF);
	tcdw_word[3] =  (TOA >> 20) & (0xFF);
	tcdw_word[4] =  (TOA >> 12) & (0xFF);
	tcdw_word[5] =  (TOA >> 4) & (0xFF);
	tcdw_word[6] =  (TOA << 4)|(tcdw.PATH << 3)|(CMD);
	tcdw_word[7] = (0x80);
	tcdw_word[8] =  (tcdw.FVAL >> 32) & (0xFF);
	tcdw_word[9] =  (tcdw.FVAL >> 24) & (0xFF);
	tcdw_word[10] = (tcdw.FVAL >> 16) & (0xFF);
	tcdw_word[11] = (tcdw.FVAL >> 8) & (0xFF);
	tcdw_word[12] = (tcdw.FVAL) & (0xFF);
	tcdw_word[13] = (L_POSNEG << 7)|(L_INT);
	tcdw_word[14] = (L_TENTH)|(L_HUND);
	tcdw_word[15] = (0x00);

}

//int main(){
//
//struct tcdw_s my_tcdw = tcdw_default;
//
//char *tcdw_word = malloc(sizeof(char *)*17);
//tcdw_constructor(tcdw_word,my_tcdw);
//
//}

