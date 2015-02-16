#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ternutils.h"

#define WIDTH 6
#define RANGE 729

int condSkip(int * tryte);
void invTryte(int * tryte);
void JP(int * tryte, int * pc);
void STO(int * reg, int * mem);
void incrTryte(int * tryte);
void addTryte(int * reg, int * tryte);

/* Decides and executes conditional skips */
int condSkip(int * tryte){
	if(0<=tern2dec(tryte)){
		return 1;
	}
	return 0;
}

/* Flips all trits in a tryte to be negative.
This inverts the tryte */
void invTryte(int * tryte){
	int i;
	for(i=0;i<WIDTH;i++){
		tryte[i]=-1*tryte[i];
	}
}

/* Places a new value in the program counter */
void JP(int *tryte, int *pc){
	/* Jump to address in following mem location */
	*pc=364+tern2dec(tryte);
}

/* Instruction to store a register value into memory */
void STO(int * reg, int * mem){
	int i;
	for(i=0;i<WIDTH;i++){
		mem[i]=reg[i];
	}
}

/* Adds 1 to a tryte */
void incrTryte(int * tryte){
	int i;
	tryte[0]++;
	for(i=0;i<WIDTH;i++){
		if (tryte[i]==2){
			if(i==WIDTH-1) puts("OVERFLOW ERROR");
			tryte[i]=-1;
			tryte[i+1]++;
		}
	}
}

/* Adds a tryte to an existing value in a register */
void addTryte(int * reg, int * tryte){
	int i;
	/* Sum trits */
	for(i=0;i<WIDTH;i++){
		reg[i] = (reg[i]) + (tryte[i]);
	}

	/* Starting from LSB, cleanup */
	for(i=0;i<WIDTH;i++){
		/* If 2 then make -1 and add 1 to next. */
		if(2==reg[i]){
			if(i==(WIDTH-1)) puts("OVERFLOW ERROR");
			reg[i]=-1;
			reg[i+1] = reg[i+1]+1;
		}else if(3==reg[i]){
			if(i==(WIDTH-1)) puts("OVERFLOW ERROR");
			reg[i]=0;
			reg[i+1] = reg[i+1]+1;
		}else if(-2==reg[i]){
			if(i==(WIDTH-1)) puts("UNDERFLOW ERROR");
			reg[i]=1;
			reg[i+1] = reg[i+1]-1;
		}else if(-3==reg[i]){
			if(i==(WIDTH-1)) puts("UNDERFLOW ERROR");
			reg[i]=0;
			reg[i+1] = reg[i+1]-1;
		}
	}
}

