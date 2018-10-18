#include <sys/socket.h>     /* socket definitions   */
#include <sys/types.h>      /* socket types         */
#include <arpa/inet.h>      /* inet functions       */
#include <unistd.h>         /* misc. UNIX functions */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Global constants */
#define MAX_LINE    	(1000)
#define CONFIRM_LINE	(200)
#define nul '\0'

int main(int argc, char *argv[])
{

	int conn_s;
	short int port;
	char buffer[MAX_LINE];
	struct sockaddr_in servaddr;
	char *endptr;
	char *szAddress;
	char *szPort;
	char *filePath;
	char *toFormat;
	char *toName;


	/* Command line arguments */
	szAddress = argv[1];
	szPort = argv[2];
	filePath = argv[3];
	toFormat = argv[4];
	toName = argv[5];

	if (argc != 6) {
		fprintf(stderr, "CLIENT: Invalid command line arguments.\n");
		exit(EXIT_FAILURE);
	}

	port = strtol(szPort, &endptr, 0);
	if (*endptr) {
		fprintf(stderr, "CLIENT: Invalid port number.\n");
		exit(EXIT_FAILURE);
	}

    /* Creating a socket */

    if ((conn_s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "CLIENT: Error creating a socket.\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    /* Setting the remote IP address */

    if (inet_aton(szAddress, &servaddr.sin_addr) <= 0) {
    	fprintf(stderr, "CLIENT: Invalid remote IP address.\n");
    	exit(EXIT_FAILURE);
    }

    /* Connecting to server TCP */

    if (connect (conn_s, (struct sockaddr *) & servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "CLIENT: Error assigning an address.\n");
        exit(EXIT_FAILURE);
    }

    int fileSize;
    FILE *inputFile = fopen(filePath, "rb");
    if (inputFile == NULL) {
    	fprintf(stderr, "Error opening the file.\n");
    	exit(EXIT_FAILURE);
    }

    /* Getting the file size */
    fseek(inputFile, 0, SEEK_END);
    fileSize = ftell(inputFile);
    //printf("%d", fileSize);
    rewind(inputFile);

    int fileNameSize = strlen(toName);

    char fileBuffer[fileSize];
    fread(fileBuffer, 1, fileSize, inputFile);

    memcpy(buffer, toFormat, 1);

    memcpy(buffer+1, &fileNameSize, sizeof(int));

    memcpy(buffer+5, toName, fileNameSize);

    memcpy(buffer+5+fileNameSize, &fileSize, sizeof(int));

    memcpy(buffer+5+fileNameSize+sizeof(int), fileBuffer, fileSize);

    write(conn_s, buffer, MAX_LINE);

    char receivedBuffer[CONFIRM_LINE];

    read(conn_s, receivedBuffer, MAX_LINE-1);

    receivedBuffer[CONFIRM_LINE] = nul;

    close(conn_s);
    
    return EXIT_SUCCESS;
}
