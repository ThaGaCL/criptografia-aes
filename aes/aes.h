#ifndef AES_H                                                                                     
#define AES_H

#include <stdio.h>
#include "gf.h"
#include "otp/otp.h"

// número de colunas (palavras de 32 bits) por matriz estado
#define NB 4
// número de palvras de 32 bits por chave
#define NK 4
// número de rodadas, depende de NK
#define NR 10

extern const char RConTable[];
void rotWord(char *word);
void subWord(char *word, char* otp_key);
void RCon_1(char i, char R[4]);
void RCon(char i, char R[4]);
void keyExpansion(char *key, char *w, char* otp_key);

void addRoundKey(char *state, char *w, char round);

void subBytes(char *state, char *otp_key);
void invSubBytes(char *state, char *otp_key);

void shiftRows(char *state);
void invShiftRows(char *state);

void mixColumns_1(char *state);
void mixColumns(char *state);
void invMixColumns(char *state);

void aesCipher(char in[4*NB], char out[4*NB], char *w, char* otp_key);
void aesInvCipher(char in[4*NB], char out[4*NB], char *w, char* otp_key);

#endif