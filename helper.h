#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP


#include <unistd.h>
#include <stdio.h>


int readTypeZero(char*, int, FILE*, char*);
int readTypeOne(char*, int, FILE*, char*);
void printZeroType(FILE *, uint8_t , uint16_t *);
void printOneType(FILE *, char* , int,  uint16_t *);

#endif