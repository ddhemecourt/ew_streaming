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
#define PORT 49152

void *thread_func(int sock)
{
    int valread;
    char* hello = "String1\n";
    char buffer[1024] = { 0 };
    send(sock, hello, strlen(hello), 0);
    //send(sock, hello, strlen(hello), 0);
    //printf("Hello message sent\n");
    //valread = read(sock, buffer, 1024);
    //printf("%s\n", buffer);
    sleep(1); 
    send(sock, hello, strlen(hello), 0);
    sleep(1);
    send(sock, hello, strlen(hello), 0);
    sleep(1);
    send(sock, hello, strlen(hello), 0);
    sleep(10);
    send(sock, hello, strlen(hello), 0);
    // closing the connected socket
    return;
}



void *thread_func2(int sock)
{
    int valread;
    char* hello = "String2\n";
    char buffer[1024] = { 0 };



    send(sock, hello, strlen(hello), 0);
    send(sock, hello, strlen(hello), 0);
    //printf("Hello message sent\n");
    //valread = read(sock, buffer, 1024);
    //printf("%s\n", buffer);
 
    // closing the connected socket
    return;
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
        ret = pthread_create(&thread, &attr, thread_func, sock);
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


