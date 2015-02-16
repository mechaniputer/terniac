#ifndef TERNUTILS_H
#define TERNUTILS_H

#define WIDTH 6
#define RANGE 729

/* ********************* */
/* Non-instruction Utils */
/* ********************* */
void zeroTryte(int * tryte);
int tern2dec(int * tryte);

/* ************ */
/* Output Utils */
/* ************ */
void dispTryte(int * tryte);
void dump(int ** mem);
void printhelp();

/* ******************** */
/* Initialization Utils */
/* ******************** */
void load(int **mem, char * filename);

#endif

