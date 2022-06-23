
/*                                                                  
 * POSIX Real Time Example
 * using a single pthread as RT thread
 */
 
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
#include "adw_constructor.h"
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
        pinfo->period_ns = 1000000000;
 
        clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}
 
static void do_rt_task(int sock, char* adw)
{
        /* Do RT stuff here. */
      	
        char* hello = "String1\n";
	//printf("%x",adw[0]);
	//printf("ADW LENGTH: %d\n", strlen(adw));
        char buffer[1024] = { 0 };
        //send(sock, hello, strlen(hello), 0);
        send(sock, adw, 32, 0);
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


	struct adw_s adw0 = {true, true, true, false, false, false, false, 0, 32768, 0, 0, 28800, 0};	
	char *adw_word_0 = malloc(sizeof(char *)*33);
//	adw_constructor(adw_word_0,adw0);


	struct adw_s adw1 = {true, true, true, false, false, false, false, 0, 32768, 0, 1, 28800, 0};	
	char *adw_word_1 = malloc(sizeof(char *)*33);
//	adw_constructor(adw_word_1,adw1);


	periodic_task_init(&pinfo);
 
        while (1) {
		printf("RT TEST\n");
		//if(val == 0){
                //	do_rt_task(sock,adw_word_0);
		//}
		//else{

                //	do_rt_task(sock,adw_word_1);
		//}
		//val ^= (0x01);
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
    	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    	    printf("\n Socket creation error \n");
    	    return;
    	}


    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(PORT);
 
    	// Convert IPv4 and IPv6 addresses from text to binary
    	// form
    	if (inet_pton(AF_INET, "192.168.1.200", &serv_addr.sin_addr)
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


