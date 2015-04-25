#include <stdio.h>
#include <assert.h>
#include <string.h>

#define WIDTH 6
#define RANGE 27
#define LOW -364

#define MAX_CMD_ARGS 2
#define BUF_LEN 5

#define LABELSIZE 4
#define NUMLABELS 45

int tryte[6];
int curr=LOW;

int mem[RANGE][WIDTH];

/* Hacky global, should move this later */
int max;

/* TODO Store all opcodes as text mnemonics, and integer DATA, ignoring comments
Pass 1: Generate LABELs ***WORKS
Pass 2: Sub integers for LABELs 
Pass 3: Sub trytes for mneumonics and trytes for integers ***PARTLY WORKS */


/* Also, all label names are 4 chars */
char labelNames[NUMLABELS][LABELSIZE];
int nextlabel=0;
int labelTrytes[NUMLABELS][WIDTH];


/* Stores conversion result in global tryte[] */
void decTern(int num){
        int i;

        for(i=0;i<WIDTH;i++){
                        tryte[i]=0;
        }
                /* 243's place */
                if(num<0){
                        if(num<(-81-27-9-3-1)){
                                tryte[5]=-1;
                                num+=243;
                        }

                }else{
                        if(num>(81+27+9+3+1)){
                                tryte[5]=1;
                                num-=243;
                        }
                }

                /* 81's place */
                if(num<0){
                        if(num<(-27-9-3-1)){
                                tryte[4]=-1;
                                num+=81;
                        }
                }else{
                        if(num>(27+9+3+1)){
                                tryte[4]=1;
                                num-=81;
                        }

                }
       
                /* 27's place */
                if(num<0){
                        if(num<(-9-3-1)){
                                tryte[3]=-1;
                                num+=27;
                        }
                }else{
                        if(num>(9+3+1)){
                                tryte[3]=1;
                                num-=27;
                        }
                }

                /* 9's place */
                if(num<0){
                        if(num<(-3-1)){
                                tryte[2]=-1;
                                num+=9;
                        }
                }else{
                        if(num>(3+1)){
                                tryte[2]=1;
                                num-=9;
                        }
                }

                /* 3's place */
                if(num<0){
                        if(num<(-1)){
                                tryte[1]=-1;
                                num+=3;
                        }
                }else{
                        if(num>(1)){
                                tryte[1]=1;
                                num-=3;
                        }
                }

                /* 1's place */
                if(num<0){
                        if(num<(0)){
                                tryte[0]=-1;
                                num+=1;
                        }
                }else{
                        if(num>(0)){
                                tryte[0]=1;
                                num-=1;
                        }
                }
        return;
}


int dispTryte(int * tryte){
	int i;
	for(i=0;i<WIDTH;i++){
		printf("%d ",tryte[i]);
	}
	printf("\n");
	return 0;
}


void parse(char * inp, char cmd[MAX_CMD_ARGS][BUF_LEN]){
	int i;
	char * start;

	for (i = 0; i < MAX_CMD_ARGS && *inp; i++) {
		while (isspace(*inp)) inp++;
		start = inp;
		while (!isspace(*inp) && *inp) inp++;
		memcpy(cmd[i], start, inp-start);
		cmd[i][inp-start] = 0;
	}

	for (; i < MAX_CMD_ARGS; i++) {
		cmd[i][0] = 0;
	}
}

int opcode(char * str){
	int i;
	char cmd[MAX_CMD_ARGS][BUF_LEN];
	if(!strncmp(str, "OUT A\n",5)){
		puts("-1 -1 0 0 0 0");
		curr++;
	}else if(!strncmp(str, "OUT B\n",5)){
		puts("0 -1 0 0 0 0");
		curr++;
	}else if(!strncmp(str, "OUT C\n",5)){
		puts("1 -1 0 0 0 0");
		curr++;

	}else if(!strncmp(str, "IN A\n",5)){
		puts("-1 0 1 0 0 0");
		curr++;
	}else if(!strncmp(str, "IN B\n",5)){
		puts("0 0 1 0 0 0");
		curr++;
	}else if(!strncmp(str, "IN C\n",5)){
		puts("0 0 1 0 0 0");
		curr++;

	}else if(!strncmp(str, "LD A\n",5)){
		puts("-1 -1 -1 0 0 0");
		curr++;
	}else if(!strncmp(str, "LD B\n",5)){
		puts("0 -1 -1 0 0 0");
		curr++;
	}else if(!strncmp(str, "LD C\n",5)){
		puts("1 -1 -1 0 0 0");
		curr++;

	/* Increment */
	}else if(!strncmp(str, "INC A\n",6)){
		puts("-1 0 -1 0 0 0");
		curr++;
	}else if(!strncmp(str, "INC B\n",6)){
		puts("0 0 -1 0 0 0");
		curr++;
	}else if(!strncmp(str, "INC C\n",6)){
		puts("1 0 -1 0 0 0");
		curr++;

	/* Skip */
	}else if(!strncmp(str, "SKP A\n",6)){
		puts("-1 1 -1 0 0 0");
		curr++;
	}else if(!strncmp(str, "SKP B\n",6)){
		puts("0 1 -1 0 0 0");
		curr++;
	}else if(!strncmp(str, "SKP C\n",6)){
		puts("1 1 -1 0 0 0");
		curr++;

	/* Misc */
	}else if(!strncmp(str, "HALT\n",5)){
		puts("-1 0 0 0 0 0");
		curr++;
	}else if(!strncmp(str, "NOOP\n",5)){
		puts("0 0 0 0 0 0");
		curr++;
	}else if(!strncmp(str, "JP\n",3)){
		puts("1 0 0 0 0 0");
		curr++;

	/* Invert */
	}else if(!strncmp(str, "INV A\n",6)){
		puts("-1 1 0 0 0 0");
		curr++;
	}else if(!strncmp(str, "INV B\n",6)){
		puts("0 1 0 0 0 0");
		curr++;
	}else if(!strncmp(str, "INV C\n",6)){
		puts("1 1 0 0 0 0");
		curr++;

	/* Store */
	}else if(!strncmp(str, "STO A\n",6)){
		puts("-1 -1 1 0 0 0");
		curr++;
	}else if(!strncmp(str, "STO B\n",6)){
		puts("0 -1 1 0 0 0");
		curr++;
	}else if(!strncmp(str, "STO C\n",6)){
		puts("1 -1 1 0 0 0");
		curr++;

	/* ADD */
	}else if(!strncmp(str, "ADD A\n",6)){
		puts("-1 1 1 0 0 0");
		curr++;
	}else if(!strncmp(str, "ADD B\n",6)){
		puts("0 1 1 0 0 0");
		curr++;
	}else if(!strncmp(str, "ADD C\n",6)){
		puts("1 1 1 0 0 0");
		curr++;
	

	/* DATA/ADDR/LABEL */
	}else if(!strncmp(str, "DATA",4)){
		parse(str,cmd);
		decTern(atoi(cmd[1]));
		dispTryte(tryte);
		curr++;

	}else if(!strncmp(str, "ADDR",4)){
		parse(str,cmd);
		/* Should print address of label */
		for(i=0;i<nextlabel;i++){
			if(!strncmp(cmd[1],labelNames[i],4)){
				decTern(labelTrytes[i][1]);
				dispTryte(tryte);
			}
		}

		curr++;

	}else if(!strncmp(str, "LABEL",5)){
		/* We already made labels */

	}else if(!strncmp(str,"#",1)){
		/* There is a bug where a long comment produces the typo error as well as a comment detection */
	}else{
		puts("ERROR: POSSIBLE TYPO");
	}

	return 0;
}


int genLabels(char * str){
	int i;
	char cmd[MAX_CMD_ARGS][BUF_LEN];

	if(!strncmp(str, "LD A\n",5)){
		curr++;
	}else if(!strncmp(str, "LD B\n",5)){
		curr++;
	}else if(!strncmp(str, "LD C\n",5)){
		curr++;

	}else if(!strncmp(str, "OUT A\n",5)){
		curr++;
	}else if(!strncmp(str, "OUT B\n",5)){
		curr++;
	}else if(!strncmp(str, "OUT C\n",5)){
		curr++;

	}else if(!strncmp(str, "IN A\n",5)){
		curr++;
	}else if(!strncmp(str, "IN B\n",5)){
		curr++;
	}else if(!strncmp(str, "IN C\n",5)){
		curr++;


	/* Increment */
	}else if(!strncmp(str, "INC A\n",6)){
		curr++;
	}else if(!strncmp(str, "INC B\n",6)){
		curr++;
	}else if(!strncmp(str, "INC C\n",6)){
		curr++;

	/* Skip */
	}else if(!strncmp(str, "SKP A\n",6)){
		curr++;
	}else if(!strncmp(str, "SKP B\n",6)){
		curr++;
	}else if(!strncmp(str, "SKP C\n",6)){
		curr++;

	/* Misc */
	}else if(!strncmp(str, "HALT\n",5)){
		curr++;
	}else if(!strncmp(str, "NOOP\n",5)){
		curr++;
	}else if(!strncmp(str, "JP\n",3)){
		curr++;

	/* Invert */
	}else if(!strncmp(str, "INV A\n",6)){
		curr++;
	}else if(!strncmp(str, "INV B\n",6)){
		curr++;
	}else if(!strncmp(str, "INV C\n",6)){
		curr++;

	/* Store */
	}else if(!strncmp(str, "STO A\n",6)){
		curr++;
	}else if(!strncmp(str, "STO B\n",6)){
		curr++;
	}else if(!strncmp(str, "STO C\n",6)){
		curr++;

	/* ADD */
	}else if(!strncmp(str, "ADD A\n",6)){
		curr++;
	}else if(!strncmp(str, "ADD B\n",6)){
		curr++;
	}else if(!strncmp(str, "ADD C\n",6)){
		curr++;
	
	/* DATA/ADDR/LABEL */
	}else if(!strncmp(str, "DATA",4)){
		curr++;

	}else if(!strncmp(str, "ADDR",4)){
		curr++;

	}else if(!strncmp(str, "LABEL",5)){
		parse(str,cmd);
		/* Instead of printing, it should save a label */

		for(i=0;i<LABELSIZE;i++){
			labelNames[nextlabel][i]=cmd[1][i];
			labelTrytes[nextlabel][1]=curr;
		}
		nextlabel++;

	}else if(!strncmp(str,"#",1)){
		/* There is a bug where a long comment produces the typo error as well as a comment detection */
	}else{

	}

	return 0;
}


int main(){
	char ch;
	char str[300][32];
	int i=0;
	int j;

	FILE * f;

	f = fopen("brainfuck.ternas", "r");
	assert(f != NULL);

	while(fgets(str[i], 31, f)){
		i++;
	}

	fclose(f);

	/* Holds one more than largest index being used */
	max=i;


	/* Generate labels */
	for(i=0;i<max;i++){
		genLabels(str[i]);	
	}


	/* modify to also do dec to tern conversions, use stdout */
	for(i=0;i<max;i++){
		opcode(str[i]);	
	}

	printf("\n");

	puts("Here are my labels");
	for(i=0;i<nextlabel;i++){
		for(j=0;j<LABELSIZE;j++){
			putchar(labelNames[i][j]);
		}

		printf(" at address %d",labelTrytes[i][1]);

		putchar('\n');
	}
	return 0;
}
