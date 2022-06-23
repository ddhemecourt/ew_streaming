// C program for the above approach
#include <stdio.h>
#include <string.h>
#include "read_file.h"
// Driver Code
void process_pdw_file(char *filename, struct emitter_s *em_arr, int *num_em)
{
    // Substitute the full file path
    // for the string file_path
   
    FILE* fp = fopen(filename, "r");
    int i;
    if (!fp)
        printf("Can't open file\n");
 
    else {
        // Here we have taken size of
        // array 1024 you can modify it
        char buffer[4096];
 
        int row = 0;
        int column = 0;
 
        while (fgets(buffer,
                     1024, fp)) {
            column = 0;
            row++;
 	    i = row-2;
            // To avoid printing of column
            // names in file can be changed
            // according to need
            if (row == 1)
                continue;
 
            // Splitting the data
            char* value = strtok(buffer, ", ");
 
            while (value) {
                // Column 1
                if (column == 0) {
                    em_arr[i].MOP = atoi(value);

                }
 
                // Column 2
                if (column == 1) {
                    em_arr[i].PRI = atoi(value);
                }
 
                // Column 3
                if (column == 2) {
		    if(em_arr[i].MOP == 4){
		    	em_arr[i].PW = 0;
		    	em_arr[i].SEGMENT_IDX = atoi(value);
		    }
		    else{
		    	em_arr[i].PW = atoi(value);
		     	em_arr[i].SEGMENT_IDX = 0; 	
		    } 
                }

		if (column == 3) {
			em_arr[i].offset = atof(value);
			printf("VAL: %s %f %f\n", value, atof(value), em_arr[i].offset);
		}

		if(column == 4){
			if(em_arr[i].MOP != 3){
				em_arr[i].EDGE_TYPE = atoi(value);
			}
			else{
				em_arr[i].EDGE_TYPE = 0;
			}	
		}

		if(column == 5){
			if(em_arr[i].MOP != 3){
				em_arr[i].RISE_TIME = atoi(value);
			}
			else{
				em_arr[i].RISE_TIME = 0;
			}	
		}
		
		if(column == 6){
			if(em_arr[i].MOP != 3){
				em_arr[i].FALL_TIME = atoi(value);
			}
			else{
				em_arr[i].FALL_TIME = 0;
			}	
		}

		if(em_arr[i].MOP == 3 && column == 7){
			em_arr[i].CHIP_WIDTH = atoi(value); 
		}

		if(em_arr[i].MOP == 3 && column == 8){
			em_arr[i].CODE = atoi(value); 
		}
		if (column == 9) {
			em_arr[i].FREQ_OFFSET = atof(value);
		}
		if (column == 10) {
			em_arr[i].LEVEL_OFFSET = atof(value);
		}
		if (column == 11) {
			em_arr[i].PHASE_OFFSET = atof(value);
		}
 
                value = strtok(NULL, ", ");
                column++;
            }
 
        }

	*num_em = row-1;
 
        // Close the file
        fclose(fp);
    }
}

void process_input_file(char *filename, struct emitter_ptr_s *em_arr, int *num_em_ptrs)
{
    // Substitute the full file path
    // for the string file_path
   
    FILE* fp = fopen(filename, "r");
    int i;
    if (!fp)
        printf("Can't open file\n");
 
    else {
        // Here we have taken size of
        // array 1024 you can modify it
        char buffer[4096];
 
        int row = 0;
        int column = 0;
	int BB_col = 0;
 
        while (fgets(buffer,
                     1024, fp)) {
            column = 0;
            row++;
 	    i = row-2;
            // To avoid printing of column
            // names in file can be changed
            // according to need
            if (row == 1)
                continue;
 
            // Splitting the data
            char* value = strtok(buffer, ", ");
 
            while (value) {
                // Column 1
                if (column == 0) {
                    em_arr[i].timestep = atoi(value);

                }
 
                // Column 2 
                if (column == 1) {
                    em_arr[i].type = malloc(sizeof(char)*30);
                    strcpy(em_arr[i].type,value);
		    printf("em_arr[i].type: %s\n", em_arr[i].type);
                }
                
		// Column 3
                if (column == 2) {
		    printf("EM1VAL = %s\n", em_arr[0].filename);
		    printf("VAL: %s I = %d\n", value, i);
                    em_arr[i].filename= malloc(sizeof(char)*30);
                    strcpy(em_arr[i].filename, value);
		    printf("em_arr[i].filename: %s\n", em_arr[i].filename);
		    printf("POST EM1VAL = %s\n", em_arr[0].filename);
                }
 
                // Column 4 
                if (column == 3) {
			BB_col = 0;
			em_arr[i].basebands = malloc(sizeof(int)*32);
                	char *BBs = strtok(value, "-");
			while(BBs){
				em_arr[i].basebands[BB_col] = atoi(BBs);
                		BBs = strtok(NULL, "-");
				BB_col++;
			}
			em_arr[i].BB_num = BB_col+1;
		    printf("em_arr[i].BB_num: %d\n", em_arr[i].BB_num);

		}
 
                value = strtok(NULL, ", ");
                column++;
            }
			printf("LOOP   i = %d  %d  %s  %s  %d\n", i,em_arr[i].timestep, em_arr[i].type,em_arr[i].filename, em_arr[i].BB_num);
 
        }

			i = 0;
			printf("LOOP2   i = %d  %d  %s  %s  %d\n", i,em_arr[i].timestep, em_arr[i].type,em_arr[i].filename, em_arr[i].BB_num);
			i = 1;
			printf("LOOP2   i = %d  %d  %s  %s  %d\n", i,em_arr[i].timestep, em_arr[i].type,em_arr[i].filename, em_arr[i].BB_num);
	//*num_em_ptrs = row-1;
	*num_em_ptrs = 2;
 
		for(int i = 0; i<row-1; i++){
			printf("i = %d  %d  %s  %d\n", i,em_arr[i].timestep, em_arr[i].filename, em_arr[i].BB_num);
		}
        // Close the file
        fclose(fp);
    }
}
int main(){

	struct emitter_ptr_s *em_arr = malloc(sizeof(struct emitter_ptr_s)*100);
	int num_em;
	process_input_file("template_schedule.csv", em_arr, &num_em);
	printf("num em : %d\n", num_em);
	for(int i = 0; i<num_em; i++){
		printf("i = %d  %d  %s  %d\n", i,em_arr[i].timestep, em_arr[i].filename, em_arr[i].BB_num);
	}
	return 0;

}
