//This program recieves 3 different types of signals sent from a 
//child proccess. The child process executes a c file named "child.c"
//where these signals are sent. The parent process recieves these signals 
//then calls handler() to recieve and handle signals
//Written by Matt Sawaged
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>  
#include <sys/types.h>
#include <assert.h>
#include <string.h>



int main(int argc, char *argv[])
{    

	void handler(int signum){
		if(signum == SIGUSR1){
			printf("Signal recieved from child: %d \n" ,signum);
		}
		if (signum == SIGHUP){
			printf("signal recieved from child: %d \n" ,signum);
		}
		else if (signum == SIGALRM){
			printf("signal recived from child: %d \n" ,signum);
		}	

	}
