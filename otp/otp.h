#ifndef OTP_H
#define OTP_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

char *encryption(char * cltext, char * key, int key_size);

char *decryption(char * cphtext, char * key, int key_size);
 
void convertToUpper(char *text, int size);

int calcStringSize(char *str);

char *generateRandKey(int key_size);

char *readEntry(const char *caminho);

#endif
