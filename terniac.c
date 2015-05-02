#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "instructions.h"
#include "ternutils.h"

#define WIDTH 6
#define RANGE 729
#define STAHP 500000

void dispTryte(int * tryte);
void dump(int ** mem);
void load(int **mem, char * filename);
int execute(int **mem, int *pc, int flow, int *areg, int *breg, int *creg);

int itryte[6];

/* Stores conversion result in global tryte[] */
void dec2tern(int num){
        int i;

        for(i=0;i<WIDTH;i++){
                        itryte[i]=0;
        }
                /* 243's place */
                if(num<0){
                        if(num<(-81-27-9-3-1)){
                                itryte[5]=-1;
                                num+=243;
                        }

                }else{
                        if(num>(81+27+9+3+1)){
                                itryte[5]=1;
                                num-=243;
                        }
                }

                /* 81's place */
                if(num<0){
                        if(num<(-27-9-3-1)){
                                itryte[4]=-1;
                                num+=81;
                        }
                }else{
                        if(num>(27+9+3+1)){
                                itryte[4]=1;
                                num-=81;
                        }

                }
       
                /* 27's place */
                if(num<0){
                        if(num<(-9-3-1)){
                                itryte[3]=-1;
                                num+=27;
                        }
                }else{
                        if(num>(9+3+1)){
                                itryte[3]=1;
                                num-=27;
                        }
                }
                /* 9's place */
                if(num<0){
                        if(num<(-3-1)){
                                itryte[2]=-1;
                                num+=9;
                        }
                }else{
                        if(num>(3+1)){
                                itryte[2]=1;
                                num-=9;
                        }
                }

                /* 3's place */
                if(num<0){
                        if(num<(-1)){
                                itryte[1]=-1;
                                num+=3;
                        }
                }else{
                        if(num>(1)){
                                itryte[1]=1;
                                num-=3;
                        }
                }

                /* 1's place */
                if(num<0){
                        if(num<(0)){
                                itryte[0]=-1;
                                num+=1;
                        }
                }else{
                        if(num>(0)){
                                itryte[0]=1;
                                num-=1;
                        }
                }
        return;
}



int main(int argc, char *argv[]){
	int i;
	int quit;
	int regdisp=0;
	int dumpyes=0;
	int flow=0;
	int cycles=0;

	/* One Program Counter */
	int lowmem = -((RANGE-1)/2);
	int * pc =malloc(sizeof(int));

	/* Three Registers */
	int areg[WIDTH];
	int breg[WIDTH];
	int creg[WIDTH];
	
	/* Verbosity settings */
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-dump"))
			dumpyes = 1;
		else if (!strcmp(argv[i],"-flow"))
			flow = 1;
		else if (!strcmp(argv[i],"-reg"))
			regdisp = 1;
		else if (!strcmp(argv[i],"-help")){
			printhelp();
			return 0;
		}
	}

	/* mem is array of pointers to arrays of ints */
	int **mem = malloc(RANGE*sizeof(int*));
	for (i=0; i<RANGE; i++){
		mem[i] = malloc(WIDTH*sizeof(int));
	}

	*pc=0;

	/* Zero everything */
	zeroTryte(areg);
	zeroTryte(breg);
	zeroTryte(creg);
	for(i=0; i<RANGE; i++) zeroTryte(mem[i]);

	/* Load from file */
	puts("Loading Program");
	load(mem, argv[1]);

	/* Show contents of whole memory space */
	if(dumpyes) dump(mem);

	/* This is the main loop */
	puts("Beginning execution...");
	while(*pc < RANGE){
		if(flow) printf("%d: ",*pc+lowmem);
		quit = (execute(mem, pc, flow, areg, breg, creg));
		cycles++;
		if(quit) break;
		assert(STAHP >= cycles);
	}

	printf("Execution finished after %d cycles.\n",cycles);
	if(dumpyes){ 
		dump(mem);
		printf("\n");
	}
	if(regdisp){
		puts("REG A");
		dispTryte(areg);
		i=tern2dec(areg);
		printf("  %dd\n",i);
		puts("REG B");
		dispTryte(breg);
		i=tern2dec(breg);
		printf("  %dd\n",i);
		puts("REG C");
		dispTryte(creg);
		i=tern2dec(creg);
		printf("  %dd\n\n",i);
	}
	return 0;
}

/* This is the messy part. This is what happens when you copy-paste one thing that works make the rest of it work */
int execute(int **mem, int *pc, int flow, int *areg, int *breg, int *creg){
	int i;
	int *tryte;
	tryte = mem[*pc];

	/* output instructions */
	if (tryte[0]==-1&&tryte[1]==-1&&tryte[2]==0){
		if(flow) puts("OUT A ");
		printf("PRINT %d\n",tern2dec(areg));
		*pc+=1;
	}else if (tryte[0]==0&&tryte[1]==-1&&tryte[2]==0){
		if(flow) puts("OUT B ");
		printf("PRINT %d\n",tern2dec(breg));
		*pc+=1;
	}else if (tryte[0]==1&&tryte[1]==-1&&tryte[2]==0){
		if(flow) puts("OUT C ");
		printf("PRINT %d\n",tern2dec(creg));
		*pc+=1;

	/* input instructions */
	}else if (tryte[0]==-1&&tryte[1]==0&&tryte[2]==1){
		if(flow) puts("IN A ");
		printf("IN A? :");
		scanf("%d",&i);
		dec2tern(i);
		for(i=0;i<WIDTH;i++){
			areg[i] = itryte[i];
		}
		*pc+=1;
	}else if (tryte[0]==0&&tryte[1]==0&&tryte[2]==1){
		if(flow) puts("IN B ");
		printf("IN B? :");
		scanf("%d",&i);
		dec2tern(i);
		for(i=0;i<WIDTH;i++){
			breg[i] = itryte[i];
		}
		*pc+=1;
	}else if (tryte[0]==1&&tryte[1]==0&&tryte[2]==1){
		if(flow) puts("IN C ");
		scanf("%d",&i);
		dec2tern(i);
		for(i=0;i<WIDTH;i++){
			creg[i] = itryte[i];
		}
		*pc+=1;

		*pc+=1;

	/* load instructions */
	}else if (tryte[0]==-1&&tryte[1]==-1&&tryte[2]==-1){
		if(flow) puts("LD A ");
		for(i=0;i<WIDTH;i++){
			areg[i]=mem[364+tern2dec(mem[*pc+1])][i];
		}
		*pc+=2;
	}else if (tryte[0]==0&&tryte[1]==-1&&tryte[2]==-1){
		if(flow) puts("LD B ");
		for(i=0;i<WIDTH;i++){
			breg[i]=mem[364+tern2dec(mem[*pc+1])][i];
		}
		*pc+=2;
	}else if (tryte[0]==1&&tryte[1]==-1&&tryte[2]==-1){
		if(flow) puts("LD C ");
		for(i=0;i<WIDTH;i++){
			creg[i]=mem[364+tern2dec(mem[*pc+1])][i];
		}
		*pc+=2;

	/* increment instructions */
	}else if (tryte[0]==-1 &&tryte[1]==0 &&tryte[2]==-1){
		if(flow) puts("INC A");
		incrTryte(areg);
		*pc+=1;
	}else if (tryte[0]==0 &&tryte[1]==0 &&tryte[2]==-1){
		if(flow) puts("INC B");
		incrTryte(breg);
		*pc+=1;
	}else if (tryte[0]==1 &&tryte[1]==0 &&tryte[2]==-1){
		if(flow) puts("INC C");
		incrTryte(creg);
		*pc+=1;

	/* conditional skip instructions */
	}else if (tryte[0]==-1&&tryte[1]==1&&tryte[2]==-1){
		if(flow) puts("SKP A ");
		if(condSkip(areg)) *pc+=3;
		else *pc+=1;
	}else if (tryte[0]==0&&tryte[1]==1&&tryte[2]==-1){
		if(flow) puts("SKP B ");
		if(condSkip(breg)) *pc+=3;
		else *pc+=1;
	}else if (tryte[0]==1&&tryte[1]==1&&tryte[2]==-1){
		if(flow) puts("SKP C ");
		if(condSkip(creg)) *pc+=3;
		else *pc+=1;

	/* 3 ADD instructions */
	}else if(tryte[0]==-1 &&tryte[1]==1 &&tryte[2]==1){
		if(flow) puts("ADD A");
		addTryte(areg,mem[364+tern2dec(mem[*pc+1])]);
		*pc+=2;

	}else if(tryte[0]==0 &&tryte[1]==1 &&tryte[2]==1){
		if(flow) puts("ADD B");
		addTryte(breg,mem[364+tern2dec(mem[*pc+1])]);
		*pc+=2;

	}else if(tryte[0]==1 &&tryte[1]==1 &&tryte[2]==1){
		if(flow) puts("ADD C");
		addTryte(creg,mem[364+tern2dec(mem[*pc+1])]);
		*pc+=2;

	/* 3 STO instructions */
	}else if(tryte[0]==-1 &&tryte[1]==-1 &&tryte[2]==1){
		if(flow) puts("STO A");
		STO(areg,mem[364+tern2dec(mem[*pc+1])]);
		*pc+=2;
	}else if(tryte[0]==0 &&tryte[1]==-1 &&tryte[2]==1){
		if(flow) puts("STO B");
		STO(breg,mem[364+tern2dec(mem[*pc+1])]);
		*pc+=2;
	}else if(tryte[0]==1 &&tryte[1]==-1 &&tryte[2]==1){
		if(flow) puts("STO C");
		STO(creg,mem[364+tern2dec(mem[*pc+1])]);
		*pc+=2;

	/* HALT Instruction */
	}else if(tryte[0]==-1 &&tryte[1]==0 &&tryte[2]==0){
		if(flow) puts("HALT");
		return 1;

	/* Bitwise invert instructions */
	}else if(tryte[0]==-1 &&tryte[1]==1 &&tryte[2]==0){
		if(flow) puts("INV A");
		invTryte(areg);
		*pc+=1;
	}else if(tryte[0]==0 &&tryte[1]==1 &&tryte[2]==0){
		if(flow) puts("INV B");
		invTryte(breg);
		*pc+=1;
	}else if(tryte[0]==1 &&tryte[1]==1 &&tryte[2]==0){
		if(flow) puts("INV C");
		invTryte(creg);
		*pc+=1;

	/* Unconditional JP instruction */
	}else if (tryte[0]==1&&tryte[1]==0&&tryte[2]==0){
		if(flow) puts("JP");
		JP(mem[*pc+1],pc);
		
	/* NOOP instruction */
	}else if (tryte[0]==0&&tryte[1]==0&&tryte[2]==0){
		if(flow) puts("NOOP");
		*pc+=1;
	}else{ 
		if(flow){
			dispTryte(tryte);
			puts("?");
		}
		*pc+=1;
	}

	return 0;
}

