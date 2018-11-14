#include <iostream>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <cstdio>
#include <string.h>
#include <fcntl.h>

using namespace std;



#define WRITEFD(fd, a) {    const char *b = a; \
                            assert(write(fd, b, strlen(b)) != -1); }
#define assertsyscall(x, y) if(!((x) y)){int err = errno; \
    fprintf(stderr, "In file %s at line %d: ", __FILE__, __LINE__); \
        perror(#x); exit(err);}


#define READ 0
#define WRITE 1

void writeAndSignal(int fileDesc, char callNum, const char *message = NULL)
{
	
	char buf[ 1 + 1 + (message == NULL ? 0 : strlen(message)) ];
    buf[0] = callNum;
    buf[1] = '\0';
    if (message != NULL)
    	strcat(buf, message);

    WRITEFD(fileDesc, buf);
    assertsyscall( kill(getppid(), SIGTRAP), != -1 );
}

int writeSignalAndReadResponse(int fileDesc[2], char callNum, char *respBuffer, size_t respSize, const char *message = NULL){
	writeAndSignal(fileDesc[WRITE], callNum, message);
	int len = read(fileDesc[READ], respBuffer, respSize);
    assertsyscall( len, != -1 );

    return len;
}

int main (int argc, char **argv)
{
	int pipes[2];
    for (int i=1; i<argc; i++){
        int fd = strtol(argv[i], NULL, 10);	

        if (i == 1)
        	pipes[READ] = fd;
        else if (i == 2)
        	pipes[WRITE] = fd;
    }

    int len;
    char returnBuf[4096];
    char messageBuf[4096];

    len = writeSignalAndReadResponse(pipes, 0x1, returnBuf, sizeof(returnBuf), NULL);
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "returning system time:\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipes, 0x4, returnBuf, sizeof(returnBuf), messageBuf);


    len = writeSignalAndReadResponse(pipes, 0x2, returnBuf, sizeof(returnBuf), NULL);
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "calling process' PCB:\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipes, 0x4, returnBuf, sizeof(returnBuf), messageBuf);


    len = writeSignalAndReadResponse(pipes, 0x3, returnBuf, sizeof(returnBuf), NULL);
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "list of all processes\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipes, 0x4, returnBuf, sizeof(returnBuf), messageBuf);


    len = writeSignalAndReadResponse(pipes, 0x4, returnBuf, sizeof(returnBuf), "Printing to stdout until Null found.\n");
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipes, 0x4, returnBuf, sizeof(returnBuf), messageBuf);

}
