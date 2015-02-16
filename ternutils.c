#include <stdio.h>
#include <assert.h>
#include "ternutils.h"

#define WIDTH 6
#define RANGE 729

void zeroTryte(int * tryte);
int tern2dec(int * tryte);

void dispTryte(int * tryte);
void dump(int ** mem);
void printhelp();

void load(int **mem, char * filename);

/* ********************* */
/* Non-instruction Utils */
/* ********************* */
void zeroTryte(int * tryte){
	int i;
	for(i=0; i<WIDTH; i++) tryte[i] = 0;
}

int tern2dec(int * tryte){
	int i;
	int placeval = 1;
	int accum = 0;

	for(i=0; i<WIDTH; i++){
		accum = accum + (tryte[i]*placeval);
		placeval = placeval*3;
	}
	return accum;
}

/* ************ */
/* Output Utils */
/* ************ */
void dispTryte(int * tryte){
	int i;
	for(i=0; i<WIDTH; i++){
		switch(tryte[i]){
		case 0:
			putchar('0');
			break;
		case -1:
			putchar('-');
			break;
		case 1:
			putchar('+');
			break;
		default:
			putchar(tryte[i]);
			break;
		}
	}
}

void dump(int ** mem){
	int i;
	printf("Memory Dump:\n");
	for(i=0; i<RANGE; i++){
		dispTryte(mem[i]);
		printf("\n");
	}
}

void printhelp(){
	puts("Specify input ternary executable as first argument");
	puts("Valid options are: -dump -flow -reg");
}


/* ******************** */
/* Initialization Utils */
/* ******************** */

void load(int **mem, char * filename){
	int i;
	FILE *f;
	f = fopen(filename, "r");
	assert(f != NULL);

	for(i=0; i<RANGE; i++){
		if(WIDTH != fscanf(f, "%d%d%d%d%d%d\n", &mem[i][0], &mem[i][1], &mem[i][2], &mem[i][3], &mem[i][4], &mem[i][5])) {
			/* Seems to prevent an error. Comment out for a good time. */
			zeroTryte(mem[i]);
		}
	}
}

