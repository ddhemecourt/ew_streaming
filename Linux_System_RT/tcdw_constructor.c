#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tcdw_constructor.h"

void* tcdw_constructor(char* tcdw_word,struct tcdw_s tcdw){
	
	unsigned long TOA = (tcdw.TOA)*(1e-6)*2.4e9;

	tcdw_word[0] =  (TOA >> 44) & (0xFF);	
	tcdw_word[1] =  (TOA >> 36) & (0xFF);
	tcdw_word[2] =  (TOA >> 28) & (0xFF);
	tcdw_word[3] =  (TOA >> 20) & (0xFF);
	tcdw_word[4] =  (TOA >> 12) & (0xFF);
	tcdw_word[5] =  (TOA >> 4) & (0xFF);
	tcdw_word[6] =  (TOA << 4)|(tcdw.PATH << 3)|(tcdw.CMD);
	tcdw_word[7] = (0x80);
	tcdw_word[8] =  (tcdw.FVAL >> 32) & (0xFF);
	tcdw_word[9] =  (tcdw.FVAL >> 24) & (0xFF);
	tcdw_word[10] = (tcdw.FVAL >> 16) & (0xFF);
	tcdw_word[11] = (tcdw.FVAL >> 8) & (0xFF);
	tcdw_word[12] = (tcdw.FVAL) & (0xFF);
	tcdw_word[13] = (tcdw.L_POSNEG << 7)|(tcdw.L_INT);
	tcdw_word[14] = (tcdw.L_TENTH)|(tcdw.L_HUND);
	tcdw_word[15] = (0x00);

}
/*
int main(){

struct tcdw_s my_tcdw = tcdw_default;

char *tcdw_word = malloc(sizeof(char *)*17);
tcdw_constructor(tcdw_word,my_tcdw);



printf("TEST\n");
for(int i = 0; i<32; i++){
	printf("%x",tcdw_word[i]);
}

}
*/
