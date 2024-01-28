#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
 
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#include "pdw_constructor.h"
#include "tcdw_constructor.h"
//#include "time_mask.h"
#include "read_file.h"
#define PORT 49152
#define num_bb 1 
#define SA struct sockaddr
#define MAX 80
#define pdw_port 49152
#define ctrl_port 5025
#define pdw_byte_len 32
#define pdws_per_packet 10
#define pdw_type 1 //basic is 1, expert is 0  TODO: probably change this naming convention

uint64_t TIME = 0;
int control_socket_idx = num_bb;
int GUI_socket_idx = num_bb+1;
int num_ports = num_bb+1;
struct sockaddr_in *serv_addr;

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


int get_time(int sock)
{	
	int valread;
	char buffer0[1024] = {0};
	char *t_str = ":SOURce1:BB:ESEQuencer:RTCI:STR:STIM?\n";
	send(sock, t_str, strlen(t_str),0);
    	printf("Time query message sent\n");
    	valread = read(sock, buffer0, 1024);
    	printf("sys t = %s\n", buffer0);
	return atoi(buffer0);
}



static void send_pdw(int sock, char* pdw_word, unsigned long inc, int num_pdws, int serv_addr_n)
{
	int i = 0;
	int bytes = pdw_byte_len*pdws_per_packet;
	while(1){
		if(num_pdws-i >= pdws_per_packet){
			send(sock, &pdw_word[pdw_byte_len*i], bytes, 0);
			//sendto(sock, &pdw_word[pdw_byte_len*i], bytes,  0, (const struct sockaddr *) &serv_addr[serv_addr_n],sizeof(serv_addr[serv_addr_n]));
		        i = i+pdws_per_packet;	
		}
		else if (num_pdws-i < pdws_per_packet){
			send(sock, &pdw_word[pdw_byte_len*i], (num_pdws-i)*pdw_byte_len, 0);
			//sendto(sock, &pdw_word[pdw_byte_len*i], bytes,  0, (const struct sockaddr *) &serv_addr[serv_addr_n],sizeof(serv_addr[serv_addr_n]));
		       	break;	
		}
		/*if(num_pdws == 0){
			char *nothing = "";
			sendto(sock, (const char *)nothing, 0,  0, (const struct sockaddr *) &serv_addr[serv_addr_n],sizeof(serv_addr[serv_addr_n]));
		}*/
	}
}
 
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
	TIME = 1000000*get_time(sock[control_socket_idx])+1000;
	printf("TIME = %d\n", TIME);

	struct period_info pinfo;
 	
	int val = 0;

	//Server buffer read vars
	char *buff = malloc(sizeof(char)*10000);
	int n = 0;
	int z;

	double sec = 1000000;
	int num_em;
	bool retune_flag[num_bb];
	int retune_time[num_bb];
	int num_pdws_out;
	struct pdw_s *pdws_out;
	char *pdw_words;
	struct emitter_s *em_arr = malloc(sizeof(struct emitter_s)*100);
	
	float *phase_dir = malloc(sizeof(float)*360);
	float *amp_dir = malloc(sizeof(float)*360);

	for(int x = 0; x<360; x++){
		phase_dir[x] = 0;
		amp_dir[x] = 0;
	}



	periodic_task_init(&pinfo);
	while (1) {
		bzero(buff, MAX);
		z = recv(sock[GUI_socket_idx], buff, 10000,0);
		if(z > 10){
			for (int i = 0; i<num_bb; i++){	
//				free(em_arr);
//				em_arr = malloc(sizeof(struct emitter_s)*100);				
				process_pdw_string(buff, em_arr, &num_em, phase_dir, amp_dir);
			}
			printf("From client: %s  %d\n", buff, z);
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

			if(i>0){
				for(int x = 0; x<num_em; x++){
					em_arr[x].PHASE_OFFSET = em_arr[x].REF_PHASE_OFFSET + phase_dir[em_arr[x].Direction];
					em_arr[x].LEVEL_OFFSET = em_arr[x].REF_LEVEL_OFFSET + amp_dir[em_arr[x].Direction];
				}
			}
			else{
				for(int x = 0; x<num_em; x++){
					em_arr[x].PHASE_OFFSET = em_arr[x].REF_PHASE_OFFSET;
					em_arr[x].LEVEL_OFFSET = em_arr[x].REF_LEVEL_OFFSET;
				}	
			}

			//printf("T0 = %d\n", TIME);
			pdws_out = emitter_to_pdws(em_arr, num_em,1000, &num_pdws_out, TIME, i);
			pdw_words = malloc(sizeof(char *)*pdw_byte_len*num_pdws_out);
			for(int j = 0; j<num_pdws_out; j++){
				pdw_constructor(pdw_words,pdws_out[j],j*pdw_byte_len,pdw_type);	
			}

			send_pdw(sock[i],pdw_words,1000, num_pdws_out,i);
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

	/*ESTABLISH CLIENT CONNECTIONS TO PDW STREAMING PORTS AND CONTROL PORT*/
//	int num_ports = num_bb+1;
//	const char *IP[] = {"192.168.58.50","192.168.58.51","192.168.58.52","192.168.58.53","192.168.58.11"};
	const char *IP[] = {"192.168.1.51","192.168.58.21"};
	int *ports = malloc(sizeof(int)*(num_ports));
	for(int u = 0; u<num_ports; u++){
	
		if(u == num_ports-1){
			ports[u] = ctrl_port;
		}
		else{
			ports[u] = pdw_port;
		}

	}
    	int *sock = malloc(sizeof(int)*4);
	int client_fd;
	int flag = 1;
	serv_addr = malloc(sizeof(struct sockaddr_in)*num_ports);
	/*for (int n = 0; n<num_ports-1; n++){
		sock[n] = n;
    		if ((sock[n] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    		    printf("\n Socket creation error \n");
    		    return;
    		}

    		serv_addr[n].sin_family = AF_INET;
    		serv_addr[n].sin_port = htons(ports[n]);
    	
		if (inet_pton(AF_INET, IP[n], &serv_addr[n].sin_addr)
    		    <= 0) {
    		    printf(
    		        "\nInvalid address/ Address not supported \n");
    		    return;
    		}

	}*/
	for (int n = 0; n<num_ports; n++){
		sock[n] = n;
    		if ((sock[n] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    		    printf("\n Socket creation error \n");
    		    return;
    		}

    		serv_addr[n].sin_family = AF_INET;
    		serv_addr[n].sin_port = htons(ports[n]);
    	
		if (inet_pton(AF_INET, IP[n], &serv_addr[n].sin_addr)
    		    <= 0) {
    		    printf(
    		        "\nInvalid address/ Address not supported \n");
    		    return;
    		}

		if(setsockopt(sock[n],
				IPPROTO_TCP,
				TCP_NODELAY, 
				(char *) &flag,
				sizeof(int))==0){
			printf("Issue setting socket option\n");
		}

    		if ((client_fd
    		     = connect(sock[n], (struct sockaddr*)&serv_addr[n],
    		               sizeof(serv_addr[n])))
    		    < 0) {
    		    printf("\nConnection Failed 0 on n = %d\n", n);
    		    return;
    		}
	}

	/*ESTABLISH SERVER SOCKET FOR USER INPUT*/
	
    	int sockfd, connfd, len;
    	struct sockaddr_in servaddr, cli;
   
    	// socket create and verification
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd == -1) {
    	    printf("socket creation failed...\n");
    	    exit(0);
    	}
    	else
    	    printf("Socket successfully created..\n");
    	bzero(&servaddr, sizeof(servaddr));
   
    	// assign IP, PORT
    	servaddr.sin_family = AF_INET;
    	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	servaddr.sin_addr.s_addr = inet_addr("192.168.58.8");
    	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.4");
    	servaddr.sin_port = htons(8080);
   
    	// Binding newly created socket to given IP and verification
    	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    	    printf("socket bind failed...\n");
    	    exit(0);
    	}
    	else
    	    printf("Socket successfully binded..\n");
   
    	// Now server is ready to listen and verification
    	if ((listen(sockfd, 5)) != 0) {
    	    printf("Listen failed...\n");
    	    exit(0);
    	}
    	else
    	    printf("Server listening..\n");
    	len = sizeof(cli);
   
    	// Accept the data packet from client and verification
    	sock[GUI_socket_idx] = accept(sockfd, (SA*)&cli, &len);
    	if (sock[GUI_socket_idx] < 0) {
    	    printf("server accept failed...\n");
    	    exit(0);
    	}
    	else
    	    printf("server accept the client...\n");

	fcntl(sock[GUI_socket_idx], F_SETFL, O_NONBLOCK);
	/*ESTABLISH HIGH PRIORITY THREAD TO EXECUTE PROGRAM*/

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


