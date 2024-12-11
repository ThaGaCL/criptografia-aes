#ifndef GF_H
#define GF_H

#include <stdio.h>

extern const char gfMult[];
#define byteMult_1(a,b) gfMult[256*a + b]
char byteMult(char a, char b);
void wordMult(char *a, char *b, char *d);

#endif