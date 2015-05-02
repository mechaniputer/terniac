#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(){
	FILE *fp;
	char erg;

	fp = fopen("fucktorial.bf", "r");
	assert(fp != NULL);

	while((erg = fgetc(fp)) != EOF){
		switch(erg){
		case '<':
			puts("DATA 0");
			break;
		case '>':
			puts("DATA -1");
			break;
		case '+':
			puts("DATA -2");
			break;
		case '-':
			puts("DATA -3");
			break;
		case '[':
			puts("DATA -4");
			break;
		case ']':
			puts("DATA -5");
			break;
		case '.':
			puts("DATA -6");
			break;
		case ',':
			puts("DATA -7");
			break;
		default:
			break;
		}
	}
	fclose(fp);
	return 0;
}

