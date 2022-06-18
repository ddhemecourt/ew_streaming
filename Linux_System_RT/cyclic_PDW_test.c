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
#include "time_mask.h"
#define PORT 49152



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
        pinfo->period_ns = 3000000000;
 
        clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}


static void send_pdw(int sock, char* pdw_word, unsigned long inc, int num_pdws)
{
	int i = 0;
	while(1){
		if(num_pdws-i >= 10){
			for(int j = 0; j<10; j++){
				pdw_inc_toa(&pdw_word[48*(i+j)], inc);	
			}
			send(sock, &pdw_word[48*i], 480, 0);
		        i = i+10;	
		}
		else if (num_pdws-i < 10){
			for(int j = 0; j<(num_pdws-i); j++){
				pdw_inc_toa(&pdw_word[48*(i+j)], inc);	
			}
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


void *simple_cyclic_task(int sock)
{
        struct period_info pinfo;
 	
	int val = 0;

	struct pdw_s pdw1 = {5000000, 0, false, false, true, 50000000, 0,0, false, 0, 20, 0, 0, 0, 0, 0, false, 0, 0};	
	char *pdw_word_1 = malloc(sizeof(char *)*49);
	pdw_constructor(pdw_word_1,pdw1,0);
		for(int n = 0; n<48; n++){
			printf("%x\n", pdw_word_1[n]);
		}

	//struct tcdw_s tcdw = tcdw_default;	
	//char *tcdw_word = malloc(sizeof(char *)*17);
	//tcdw_constructor(tcdw_word,tcdw);
	//periodic_task_init(&pinfo);
 
	struct emitter_s em_arr[3]; 
	em_arr[0] = (struct emitter_s){0, 50, 500};
	em_arr[1] = (struct emitter_s){0, 5, 40};
	em_arr[2] = (struct emitter_s){0, 10, 90};
	int num_pdws_out;
	struct pdw_s *pdws_out = emitter_to_pdws(em_arr, 3,1000, &num_pdws_out);
	char *pdw_word = malloc(sizeof(char *)*48);
	char *pdw_words = malloc(sizeof(char *)*48*num_pdws_out);
	printf("NUM_PDWS: %d\n", num_pdws_out);
	int i = 0;	
	for(int i = 0; i<num_pdws_out; i++){
		printf("TOA: %d  TON: %d\n", pdws_out[i].TOA,pdws_out[i].TON);
		pdw_constructor(pdw_words,pdws_out[i],i*48);	
//	//	for(int n = 0; n<48; n++){
//	//		printf("%x\n", pdw_word[n]);
//	//	}
	}

	
	//send_pdw(sock,pdw_words,0, num_pdws_out);


        //send_pdw(sock,pdw_word,0);
        //send_tcdw(sock,tcdw_word);
        //send_pdw(sock,pdw_word_1,11000);
	while (1) {
		//send_pdw(sock,pdw_words,2000, num_pdws_out);
                printf("PERIODi\n");
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

    	int sock = 0, client_fd;
    	struct sockaddr_in serv_addr;
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	    printf("\n Socket creation error \n");
    	    return;
    	}


    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(PORT);
 
    	// Convert IPv4 and IPv6 addresses from text to binary
    	// form
    	if (inet_pton(AF_INET, "192.168.58.200", &serv_addr.sin_addr)
    	    <= 0) {
    	    printf(
    	        "\nInvalid address/ Address not supported \n");
    	    return;
    	}
 
    	if ((client_fd
    	     = connect(sock, (struct sockaddr*)&serv_addr,
    	               sizeof(serv_addr)))
    	    < 0) {
    	    printf("\nConnection Failed \n");
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


