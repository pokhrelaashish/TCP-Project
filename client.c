#include <sys/socket.h>     /* socket definitions   */
#include <sys/types.h>      /* socket types         */
#include <arpa/inet.h>      /* inet functions       */
#include <unistd.h>         /* misc. UNIX functions */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Global constants */
#define MAX_LINE    (1000)


int main(int argc, char *argv[])
{

	int conn_s;
	short int port;
	struct sockaddr_in servaddr;
	char buff[MAX_LINE];
	char *endptr;
	char *szAddress;
	char *szPort;

	szAddress = argv[1];
	szPort = argv[2];

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
    	fprintf(stderr, "Client: Invalid remote IP address.\n");
    	exit(EXIT_FAILURE);
    }

    /* Connecting to server TCP */

    if (connect (conn_s, (struct sockaddr *) & servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "CLIENT: Error assigning an address.\n");
        exit(EXIT_FAILURE);
    }

    close(conn_s);
    
    
}
