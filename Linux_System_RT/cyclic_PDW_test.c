#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
 
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#include "pdw_constructor.h"
#include "tcdw_constructor.h"
//#include "time_mask.h"
#include "read_file.h"
#define PORT 49152
#define num_bb 2
uint64_t TIME = 0;

struct period_info {
        struct timespec next_period;
        long period_ns;
};
 
static void inc_period(struct period_info *pinfo) 
{
        pinfo->next_period.tv_nsec += pinfo->period_ns;
 
        while (pinfo->next_period.tv_nsec >= 1000000000) {
                /* timespec nsec overflow */
                pinfo->next_period.tv_sec++;
                pinfo->next_period.tv_nsec -= 1000000000;
        }
}
 
static void periodic_task_init(struct period_info *pinfo)
{
        /* for simplicity, hardcoding a 1ms period */
        pinfo->period_ns = 1000000;
 
        clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}


static void send_pdw(int sock, char* pdw_word, unsigned long inc, int num_pdws)
{
	int i = 0;
	while(1){
		if(num_pdws-i >= 10){
			//for(int j = 0; j<10; j++){
			//	pdw_inc_toa(&pdw_word[48*(i+j)], inc);	
			//}
			send(sock, &pdw_word[48*i], 480, 0);
		        i = i+10;	
		}
		else if (num_pdws-i < 10){
			//for(int j = 0; j<(num_pdws-i); j++){
			//	pdw_inc_toa(&pdw_word[48*(i+j)], inc);	
			//}
			send(sock, &pdw_word[48*i], (num_pdws-i)*48, 0);
		       	break;	
		}
	}
}
//static void send_pdw(int sock, char* pdw_word, unsigned long inc)
//{
////	pdw_inc_toa(pdw_word[48*i], inc);
//        send(sock, pdw_word, 48, 0);
//}
 
static void send_tcdw(int sock, char* tcdw_word)
{
	unsigned long inc = 1000;	
	//pdw_inc_toa(tcdw_word, inc);
        send(sock, tcdw_word, 16, 0);
}

static void wait_rest_of_period(struct period_info *pinfo)
{
        inc_period(pinfo);
 
        /* for simplicity, ignoring possibilities of signal wakes */
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}


void *simple_cyclic_task(int *sock)
{
        struct period_info pinfo;
 	
	int val = 0;

	struct emitter_ptr_s *em_ptr_arr = malloc(sizeof(struct emitter_ptr_s)*100);
	int num_em_ptrs;
	process_input_file("template_schedule_basic.csv", em_ptr_arr, &num_em_ptrs);
	int em_ptr_i = 0;

	double sec = 1000000;
	int num_em[num_bb];
	bool retune_flag[num_bb];
	int retune_time[num_bb];
	int num_pdws_out;
	struct pdw_s *pdws_out;
	char *pdw_words;
	struct emitter_s **em_arr = malloc(sizeof(struct emitter_s *)*num_bb);
	for(int i = 0; i<num_bb; i++){
		em_arr[i] = malloc(sizeof(struct emitter_s)*100);
		retune_time[i] = 0;
		retune_flag[i] = false;
	}
		
	int last_t = em_ptr_arr[num_em_ptrs-1].timestep;
	periodic_task_init(&pinfo);
	while (1) {
		if(em_ptr_arr[em_ptr_i].timestep == TIME && !strcmp(em_ptr_arr[em_ptr_i].type,"Emitter")){
//			printf("GOT EMITTER\n");
			for (int i = 0; i<num_bb; i++){	
				free(em_arr[i]);
				em_arr[i] = malloc(sizeof(struct emitter_s)*100);				
				if(em_ptr_arr[em_ptr_i].basebands[i] == 1){
					process_pdw_file(em_ptr_arr[em_ptr_i].filename, em_arr[i], &num_em[i]);
				}
			}
			em_ptr_arr[em_ptr_i].timestep = em_ptr_arr[em_ptr_i].timestep + last_t + 10000;
			em_ptr_i++;
			if(em_ptr_i == num_em_ptrs){
				em_ptr_i = 0;
			}
			continue;

		}
		else if(em_ptr_arr[em_ptr_i].timestep == TIME && !strcmp(em_ptr_arr[em_ptr_i].type,"TCDW")){	
//			printf("GOT TCDW\n %d\n", em_ptr_i);	
			
			struct tcdw_s tcdw = {TIME,em_ptr_arr[em_ptr_i].PATH, em_ptr_arr[em_ptr_i].CMD, em_ptr_arr[em_ptr_i].FVAL, em_ptr_arr[em_ptr_i].LVAL};	
			char *tcdw_word = malloc(sizeof(char *)*17);
			tcdw_constructor(tcdw_word,tcdw);
			if(em_ptr_arr[em_ptr_i].basebands[0] == 1){
//				printf("Sending on 0\n");
				send_tcdw(sock[0],tcdw_word);
				retune_flag[0] = true;
			}
			if(em_ptr_arr[em_ptr_i].basebands[1] == 1){
//				printf("Sending on 1\n");
				send_tcdw(sock[1],tcdw_word);
				retune_flag[1] = true;
			}

			em_ptr_arr[em_ptr_i].timestep = em_ptr_arr[em_ptr_i].timestep + last_t + 10000;
			//for(int x = 0; x < 10; x++){
			//	wait_rest_of_period(&pinfo);
			//	TIME = TIME+1000;
			//}
			em_ptr_i++;
			if(em_ptr_i == num_em_ptrs){
				em_ptr_i = 0;
			}
			continue;
		}


		for(int i = 0; i<num_bb; i++){
			if(retune_flag[i] == true){
				retune_time[i]++;
				if(retune_time[i] == 10){
					retune_flag[i] = false;
					retune_time[i] = 0;
				}
				continue;
			}
			pdws_out = emitter_to_pdws(em_arr[i], num_em[i],1000, &num_pdws_out, TIME);
			pdw_words = malloc(sizeof(char *)*48*num_pdws_out);
			for(int j = 0; j<num_pdws_out; j++){
				pdw_constructor(pdw_words,pdws_out[j],j*48);	
			}

			send_pdw(sock[i],pdw_words,1000, num_pdws_out);
                	free(pdw_words);
			free(pdws_out);
		}	
		if(sec/TIME == 1){
			printf("TIME = %d  %ld \n", TIME, pdws_out[0].TOA);
			sec = sec+1000000;
		}	
		TIME = TIME+1000;
		wait_rest_of_period(&pinfo);

        }

        return NULL;
}



int main(int argc, char* argv[])
{
        struct sched_param param, param2;
        pthread_attr_t attr,attr2;
        pthread_t thread, thread2;
        int ret;
	int num_ports = 3;

    	int *sock = malloc(sizeof(int)*3);
	int client_fd;
    	struct sockaddr_in *serv_addr = malloc(sizeof(struct sockaddr_in)*num_ports);
	for (int n = 0; n<num_ports; n++){
		sock[n] = n;
    		if ((sock[n] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    		    printf("\n Socket creation error \n");
    		    return;
    		}

    		serv_addr[n].sin_family = AF_INET;
    		serv_addr[n].sin_port = htons(PORT);

	}


    	// Convert IPv4 and IPv6 addresses from text to binary
    	// form
    	if (inet_pton(AF_INET, "192.168.1.200", &serv_addr[0].sin_addr)
    	    <= 0) {
    	    printf(
    	        "\nInvalid address/ Address not supported \n");
    	    return;
    	}
    	if (inet_pton(AF_INET, "192.168.58.201", &serv_addr[1].sin_addr)
    	    <= 0) {
    	    printf(
    	        "\nInvalid address/ Address not supported \n");
    	    return;
    	}
 

    	if ((client_fd
    	     = connect(sock[0], (struct sockaddr0*)&serv_addr[0],
    	               sizeof(serv_addr[0])))
    	    < 0) {
    	    printf("\nConnection Failed 0\n");
    	    return;
    	}
    	if ((client_fd
    	     = connect(sock[1], (struct sockaddr1*)&serv_addr[1],
    	               sizeof(serv_addr[1])))
    	    < 0) {
    	    printf("\nConnection Failed 1\n");
    	    return;
    	}

        /* Lock memory */
        if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
                printf("mlockall failed: %m\n");
                exit(-2);
        }
 
        /* Initialize pthread attributes (default values) */
        ret = pthread_attr_init(&attr);
        if (ret) {
                printf("init pthread attributes failed\n");
                goto out;
        }
 
        ret = pthread_attr_init(&attr2);
        if (ret) {
                printf("init pthread attributes failed\n");
                goto out;
        }

        /* Set a specific stack size  */
        ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
        if (ret) {
            printf("pthread setstacksize failed\n");
            goto out;
        }

        ret = pthread_attr_setstacksize(&attr2, PTHREAD_STACK_MIN);
        if (ret) {
            printf("pthread setstacksize failed\n");
            goto out;
        }

        /* Set scheduler policy and priority of pthread */
        ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        if (ret) {
                printf("pthread setschedpolicy failed\n");
                goto out;
        }

        ret = pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
        if (ret) {
                printf("pthread setschedpolicy failed\n");
                goto out;
        }


	param.sched_priority = 99;
        ret = pthread_attr_setschedparam(&attr, &param);
        if (ret) {
                printf("pthread setschedparam failed\n");
                goto out;
        }


	param2.sched_priority = 99;
        ret = pthread_attr_setschedparam(&attr2, &param2);
        if (ret) {
                printf("pthread setschedparam failed\n");
                goto out;
        }

        /* Use scheduling parameters of attr */
        ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        if (ret) {
                printf("pthread setinheritsched failed\n");
                goto out;
        }


        ret = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
        if (ret) {
                printf("pthread setinheritsched failed\n");
                goto out;
        }

        /* Create a pthread with specified attributes */
        ret = pthread_create(&thread, &attr, simple_cyclic_task, sock);
        if (ret) {
                printf("create pthread failed with code %d \n", ret);
		goto out;
        }

 
        /* Create a pthread with specified attributes */
        //ret = pthread_create(&thread2, &attr2, thread_func2, sock);
        //if (ret) {
        //        printf("create pthread failed with code %d \n", ret);
	//	goto out;
        //}
        /* Join the thread and wait until it is done */
        ret = pthread_join(thread, NULL);
        if (ret)
                printf("join pthread failed: %m\n");
 
        /* Join the thread and wait until it is done */
        //ret = pthread_join(thread2, NULL);
        //if (ret)
        //        printf("join pthread failed: %m\n");
 
    	close(client_fd);
out:
        return ret;
}


