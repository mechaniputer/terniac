#include <stdio.h>
#include <assert.h>

#define WIDTH 6
#define RANGE 729
#define STAHP 3000

/* TODO 
 * Memory-mapped I/O
 * Cleanup
 * Commandline options */

char inp[WIDTH];
int a[WIDTH];
int b[WIDTH];
int c[WIDTH];
int mem[RANGE][WIDTH];
int pc=0;
int instructions=0;

/* Convert tryte integer array to an integer */
int ternDec(int * tryte){
	int i;
	int sig=1;
	int acc=0;
	for(i=0;i<WIDTH;i++){
		acc=acc+(tryte[i]*sig);
		sig=sig*3;
	}
	return acc;
}

/* Decides and executes conditional skips */
void condSkip(int * tryte){
	int i;
	dispTryte(tryte);
	if(0<=ternDec(tryte)){
		printf(" is Nonnegative, Skipping next Instruction\n");
		pc+=2;
	}else{
		printf(" is Negative, not skipping\n");
	}
	pc++;
	return;
}

/* Flips all trits in a tryte to be negative.
This inverts the tryte */
void invTryte(int * tryte){
	int i;
	for(i=0;i<WIDTH;i++){
		tryte[i]=-1*tryte[i];
	}
	return;
}

/* Places a new value in the program counter */
void JP(){
	printf(" to %d\n",ternDec(mem[pc+1]));
	/* Jump to address in following mem location */
	pc=364+ternDec(mem[pc+1]);
	return;
}

/* Instruction to store a register value into memory */
void STO(int * reg, int * mem){
	int i;
	for(i=0;i<WIDTH;i++){
		mem[i]=reg[i];
	}
	return;
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
	return;
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
	return;
}

/* Prints a tryte in a slightly readable way */
int dispTryte(int * tryte){
	int i;
	for(i=0;i<WIDTH;i++){
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
	return 0;
}

/* Sets a tryte equal to zero */
void zeroTryte(int * tryte){
	int i;
	for(i=0;i<WIDTH;i++){
		tryte[i]=0;
	}
	return;
}

/* This reads an input file and assumes that it is a valid terniac program */
int load(char * filename){
	int i;
	FILE * f;
	f = fopen(filename, "r");
	assert(f != NULL);

	for(i=0;i<RANGE;i++){
		if(WIDTH != fscanf(f, " %d%d%d%d%d%d\n", &mem[i][0], &mem[i][1], &mem[i][2], &mem[i][3], &mem[i][4], &mem[i][5])){
			/* This prevents a strange error but complicates changing data width*/
			zeroTryte(mem[i]);
			return 0;
		}
	}

	fclose(f);
	return 0;
}

/* This is the messy part. This is what happens when you copy-paste one thing that works to make the rest of it work */
int execute(int * tryte){
	int i;

	/* load instructions */
	if (tryte[0]==-1&&tryte[1]==-1&&tryte[2]==-1){
		printf("LD A ");
		for(i=0;i<WIDTH;i++){
			a[i]=mem[364+ternDec(mem[pc+1])][i];
		}
		dispTryte(b);
		printf("\n");
		pc+=2;
	}else if (tryte[0]==0&&tryte[1]==-1&&tryte[2]==-1){
		printf("LD B ");
		for(i=0;i<WIDTH;i++){
			b[i]=mem[364+ternDec(mem[pc+1])][i];
		}
		dispTryte(b);
		printf("\n");
		pc+=2;
	}else if (tryte[0]==1&&tryte[1]==-1&&tryte[2]==-1){
		printf("LD C ");
		for(i=0;i<WIDTH;i++){
			c[i]=mem[364+ternDec(mem[pc+1])][i];
		}
		dispTryte(b);
		printf("\n");
		pc+=2;

	/* increment instructions */
	}else if (tryte[0]==-1 &&tryte[1]==0 &&tryte[2]==-1){
		puts("INC A");
		incrTryte(a);
		pc+=1;
	}else if (tryte[0]==0 &&tryte[1]==0 &&tryte[2]==-1){
		puts("INC B");
		incrTryte(b);
		pc+=1;
	}else if (tryte[0]==1 &&tryte[1]==0 &&tryte[2]==-1){
		puts("INC C");
		incrTryte(c);
		pc+=1;
	/* conditional skip instructions */
	}else if (tryte[0]==-1&&tryte[1]==1&&tryte[2]==-1){
		printf("SKP A ");
		condSkip(a);
	}else if (tryte[0]==0&&tryte[1]==1&&tryte[2]==-1){
		printf("SKP B ");
		condSkip(b);
	}else if (tryte[0]==1&&tryte[1]==1&&tryte[2]==-1){
		printf("SKP C ");
		condSkip(c);

	/* 3 ADD instructions */
	}else if(tryte[0]==-1 &&tryte[1]==1 &&tryte[2]==1){
		puts("ADD A");
		addTryte(a,mem[364+ternDec(mem[pc+1])]);
		pc+=2;

	}else if(tryte[0]==0 &&tryte[1]==1 &&tryte[2]==1){
		puts("ADD B");
		addTryte(b,mem[364+ternDec(mem[pc+1])]);
		pc+=2;

	}else if(tryte[0]==1 &&tryte[1]==1 &&tryte[2]==1){
		puts("ADD C");
		addTryte(c,mem[364+ternDec(mem[pc+1])]);
		pc+=2;


	/* 3 STO instructions */
	}else if(tryte[0]==-1 &&tryte[1]==-1 &&tryte[2]==1){
		puts("STO A");
		STO(a,mem[364+ternDec(mem[pc+1])]);
		pc+=2;
	}else if(tryte[0]==0 &&tryte[1]==-1 &&tryte[2]==1){
		printf("STO B");
		printf(" at %d\n",364+ternDec(mem[pc+1]));

		STO(b,mem[364+ternDec(mem[pc+1])]);
		pc+=2;
	}else if(tryte[0]==1 &&tryte[1]==-1 &&tryte[2]==1){
		puts("STO C");
		STO(c,mem[364+ternDec(mem[pc+1])]);
		pc+=2;

	/* HALT Instruction */
	}else if(tryte[0]==-1 &&tryte[1]==0 &&tryte[2]==0){
		puts("HALT");
		return 1;

	/* Bitwise invert instructions */
	}else if(tryte[0]==-1 &&tryte[1]==1 &&tryte[2]==0){
		puts("INV A");
		invTryte(a);
		pc++;
	}else if(tryte[0]==0 &&tryte[1]==1 &&tryte[2]==0){
		puts("INV B");
		invTryte(b);
		pc++;
	}else if(tryte[0]==1 &&tryte[1]==1 &&tryte[2]==0){
		puts("INV C");
		invTryte(c);
		pc++;

	/* Unconditional JP instruction */
	}else if (tryte[0]==1&&tryte[1]==0&&tryte[2]==0){
		printf("JP");
		JP();
		
	/* NOOP instruction */
	}else if (tryte[0]==0&&tryte[1]==0&&tryte[2]==0){
		puts("NOOP");
		pc++;
	}else{ 
		dispTryte(tryte);
		puts("?");
		pc++;
	}

	return 0;
}

void dump(){
	int i;
	printf("Memory dump\n");
	for(i=0;i<RANGE;i++){
		dispTryte(mem[i]);
		printf("\n");
	}
	return;
}

int main(int argc, char *argv[]){
	int i=0;
	int quit=0;
	printf("Blanking memory\n");
	zeroTryte(a);
	zeroTryte(b);
	zeroTryte(c);

	for(i=0;i<RANGE;i++){
		zeroTryte(mem[i]);
	}

	printf("Loading program\n");
	load(argv[1]);

	/* Show contents of whole memory space */
	dump();

	printf("\nBeginning execution\n");
	while(pc<RANGE){
		printf("%d: ",pc-364);
		quit=execute(mem[pc]);
		/* This is just to get an idea of a program's efficiency */
		instructions++;
		/* This stops it if it runs too long */
		if(instructions>=STAHP) assert(0);
		if(quit==1) break;
	}
	printf("END \n\nExecuted %d instructions\n",instructions);
	puts("REG A");
	dispTryte(a);
	i=ternDec(a);
	printf("  %dd\n",i);
	puts("REG B");
	dispTryte(b);
	i=ternDec(b);
	printf("  %dd\n",i);
	puts("REG C");
	dispTryte(c);
	i=ternDec(c);
	printf("  %dd\n\n",i);
	dump();
	return 0;
}
