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

    	struct sigaction action;
    	action.sa_handler = handler;
	sigemptyset (&action.sa_mask);
	assert (sigaction (SIGUSR1,&action,NULL)==0);
	assert (sigaction(SIGHUP,&action,NULL)==0);
	assert (sigaction(SIGALRM,&action,NULL)==0); //modified code from Dr.Beaty

     
    int pid = fork();

    if(pid > 0 ){
	int childProc = 0;
	wait(&childProc);

	
    }
    
    if(pid == 0){ //cloned process opens child.c
        execl("/home/matthew/Desktop/child", "child", (char *)NULL);


    }
    
    else if(pid <0){
        printf("something messed up"); //in parent, child couldnt be made
    }

	exit(EXIT_SUCCESS);
}
