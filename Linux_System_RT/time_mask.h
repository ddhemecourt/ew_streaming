
struct emitter_s {
	float offset; //offset in microseconds
	unsigned long PW; //us
	unsigned long PRI; //us	
	int FREQ_OFFSET;
	float REF_LEVEL_OFFSET;
	float LEVEL_OFFSET;
	float REF_PHASE_OFFSET;
	float PHASE_OFFSET;
	int MOP;
	unsigned int SEGMENT_IDX: 24;
	bool EDGE_TYPE;
	unsigned int RISE_TIME : 22;
	unsigned int FALL_TIME : 22;
	unsigned long CHIP_WIDTH : 44;
	unsigned int CODE : 4;	
	float FREQ_INC;	
	//	char *MOP; // 'UNMOD', 'LFM', 'TFM', 'Barker' , 'Arb'
	int Direction;
	unsigned long CPI;
	unsigned long Burst_Len;
	int BURSTED;
	float CPI_offset;
};



void pdw_sort(struct pdw_s *arr_out, struct pdw_s *arr, int Size[], int num_emitters, int num_pdws);


int time_mask(struct pdw_s pdw, uint64_t *Trise_arr, uint64_t *Tfall_arr, int len);


struct pdw_s *emitter_to_pdws(struct emitter_s *em, int num_emitters, double us_len, int *num_pdws_out, uint64_t T0, int bb_num);
