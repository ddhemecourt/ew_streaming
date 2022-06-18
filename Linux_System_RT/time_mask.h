
struct emitter_s {
	unsigned long offset_us;
	unsigned long PW; //us
	unsigned long PRI; //us
//	char *MOP; // 'UNMOD', 'LFM', 'TFM', 'Barker' , 'Arb'
	
};



static void pdw_sort(struct pdw_s *arr_out, struct pdw_s *arr, int Size[], int num_emitters, int num_pdws);


int time_mask(struct pdw_s pdw, uint64_t *Trise_arr, uint64_t *Tfall_arr, int len);


struct pdw_s *emitter_to_pdws(struct emitter_s *em, int num_emitters, double us_len, int *num_pdws_out);
