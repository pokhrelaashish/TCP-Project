#include <sys/socket.h>     /* socket definitions   */
#include <sys/types.h>      /* socket types         */
#include <arpa/inet.h>      /* inet functions       */
#include <unistd.h>         /* misc. UNIX functions */

#include "helper.h"         /* helper functions     */
#include <stdlib.h>
#include <stdio.h>

/* Global constants */

#define ECHO_PORT   (2002)
#define MAX_LINE    (1000)

struct sockaddr {
	uint8_t			sa_len;
	sa_family_t		sa_family;
	char			sa_data[14];
};

struct sockaddr_in {
	uint8_t			sin_len;
	sa_family_t		sin_family;
	in_port_t		sin_port;
	struct in_addr	sin_addr;
	char			sin_zero[8];
};

struct in_addr_t {
	in_addr_t		s_addr;
};

uint16_t htons(uint16_t);
uint16_t ntohs(uint16_t);

uint32_t htonl(uint32_t);
uint32_t ntohl(uint32_t);


int main(int argc, char *argv[])
{

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
        
    }

    /* Assigning an address to a socket */

    if (bind (list_s, (struct sockaddr *) & servaddr, sizeof(servaddr)) < 0) {
        
    }

    /* Listening for connection requests */

    if (listen (list_s, LISTENQ) < 0) {

    }
    
    while (1) {

        /* Start handling a connection */
    	if ((conn_s = accept (list_s, NULL, NULL)) < 0) {

    	}
    	Readline (conn_s, buffer, MAX_LINE-1);
    	Writeline (conn_s, buffer, strlen(buffer));

        /* Terminating a TCP connection */
    	if (close (conn_s) < 0) {
    		
    	}
    }

    printf("Hello World");

    return 0;
}
