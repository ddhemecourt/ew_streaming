#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "adw_constructor.h"

void* adw_constructor(char* adw_word,struct adw_s adw){
	
	adw_word[0] = 0x00;
	adw_word[1] = 0x00;
	adw_word[2] = 0x00;
	adw_word[3] = 0x00;
	adw_word[4] = 0x00;
	adw_word[5] = 0x00;
	adw_word[6] = (0x00)|(adw.SEG << 3)|(adw.USE_EXTENSION << 2);
	adw_word[7] = (0x00)|(adw.SEG_INTERRUPT << 6)|(false << 5)|(adw.IGNORE_ADW << 4)|(false << 3)|(adw.M3 << 2)|(adw.M2 << 1)|(adw.M1);
	adw_word[8] =  (adw.FREQ_OFFSET >> 24) & (0xFF);
	adw_word[9] =  (adw.FREQ_OFFSET >> 16) & (0xFF);
	adw_word[10] = (adw.FREQ_OFFSET >> 8) & (0xFF);
	adw_word[11] = (adw.FREQ_OFFSET) & (0xFF);
	adw_word[12] = (adw.LEVEL_OFFSET >> 8) & (0xFF);
	adw_word[13] = (adw.LEVEL_OFFSET) & (0xFF);
	adw_word[14] = (adw.PHASE_OFFSET >> 8) & (0xFF);
	adw_word[15] = (adw.PHASE_OFFSET) & (0xFF);
	adw_word[16] = (adw.SEGMENT_IDX >> 16) & (0xFF);
	adw_word[17] = (adw.SEGMENT_IDX >> 8) & (0xFF);
	adw_word[18] = (adw.SEGMENT_IDX) & (0xFF);
	adw_word[19] = 0x00;
	adw_word[20] = 0x00;
	adw_word[21] = 0x00;
	adw_word[22] = 0x00;
	adw_word[23] = 0x00;
	adw_word[24] = 0x00;
	adw_word[25] = 0x00;
	adw_word[26] = (adw.BURST_SRI >> 24) & (0xFF);
	adw_word[27] = (adw.BURST_SRI >> 16) & (0xFF);
	adw_word[28] = (adw.BURST_SRI >> 8) & (0xFF);
	adw_word[29] = (adw.BURST_SRI) & (0xFF);
	adw_word[30] = (adw.BURST_ADD_SEGMENTS >> 8) & (0xFF);
	adw_word[31] = (adw.BURST_ADD_SEGMENTS) & (0xFF);

}
/*
int main(){

struct adw_s my_adw = adw_default;

char *adw_word = malloc(sizeof(char *)*33);
adw_constructor(adw_word,my_adw);



printf("TEST\n");
for(int i = 0; i<32; i++){
	printf("%x",adw_word[i]);
}

}
*/
