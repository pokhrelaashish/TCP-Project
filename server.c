#include <sys/socket.h>     /* socket definitions   */
#include <sys/types.h>      /* socket types         */
#include <arpa/inet.h>      /* inet functions       */
#include <unistd.h>         /* misc. UNIX functions */

#include "helper.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

/* Global constants */

#define ECHO_PORT       (2002)
#define MAX_LINE        (1000)
#define CONFIRM_LINE    (200)
#define nul '\0'
#define LISTENQ         (1024)


int main(int argc, char *argv[])
{
    int list_s, conn_s;
    short int port;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAX_LINE];
    char *endptr;
    int status;

    if (argc == 2) {
        port = strtol(argv[1], &endptr, 0);
        if (*endptr) {
            fprintf(stderr, "SERVER: Invalid port number.\n");
            exit(EXIT_FAILURE);  
        }
    }
    else if (argc < 2) {
        port = ECHO_PORT;
    }
    else {
        fprintf(stderr, "SERVER: Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

    /* Creating a socket */

    if ((list_s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "SERVER: Error creaitng a socket.\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Assigning an address to a socket */

    if (bind (list_s, (struct sockaddr *) & servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "SERVER: Error assigning an address.\n");
        exit(EXIT_FAILURE);
    }

    /* Listening for connection requests */

    if (listen (list_s, LISTENQ) < 0) {
        fprintf(stderr, "SERVER: Error listening for connection requests.\n");
        exit(EXIT_FAILURE);
    }
    
    while (1) {

        /* Start handling a connection */
    	if ((conn_s = accept (list_s, NULL, NULL)) < 0) {
            fprintf(stderr, "SERVER: Error handling a connection.\n");
            exit(EXIT_FAILURE);
    	}

        /* Getting the file size 
        fseek(inputFile, 0, SEEK_END);
        fileSize = ftell(inputFile);
        //printf("%d", fileSize);
        rewind(inputFile); */

    	read(conn_s, buffer, MAX_LINE);

        char formatType[2];
        formatType[0] = buffer[0];
        formatType[1] = nul;

        char fileNameLen[4];
        memcpy(fileNameLen, buffer+1, sizeof(int));

        int fileNameLength = *fileNameLen;

        char targetFile[fileNameLength+1];
        memcpy(targetFile, buffer+5, fileNameLength);
        targetFile[fileNameLength];

        char fileLength[4];
        memcpy(fileLength, buffer+5+fileNameLength, 4);
        int lenFile = *fileLength;

        char fileContent[lenFile];
        memcpy(fileContent, buffer+5+fileNameLength+4, lenFile);

        FILE *targetFilePtr = fopen(targetFile, "ab+");
        int currPosition = 0;
        while (currPosition < lenFile - 1) {
            if (fileContent[0] != 0 || fileContent[1] != 1) {
                status = -1;
                break;
            }

            if (fileContent[currPosition] == 0) {
                currPosition = readTypeZero(fileContent, currPosition, targetFilePtr, formatType);
            }
            if (fileContent[currPosition] == 1) {
                currPosition = readTypeOne(fileContent, currPosition, targetFilePtr, formatType);
            }
        }

        fclose(targetFilePtr);

        char confirmationMes[CONFIRM_LINE];
        if (status == -1) {
            strncpy(confirmationMes, "Format error", CONFIRM_LINE);
        }
        else {
            strncpy(confirmationMes, "Successfully translated", CONFIRM_LINE);
        }

        confirmationMes[CONFIRM_LINE] = nul;

    	write(conn_s, buffer, strlen(buffer));

        /* Terminating a TCP connection */
    	if (close (conn_s) < 0) {
    		fprintf(stderr, "SERVER: Error terminating a TCP connection.\n");
            exit(EXIT_FAILURE);
    	}
        exit(0);
    }
}
