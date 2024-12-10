// baseado em https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf

#include "aes.h"

// desloca cada byte de uma palavra 1 posição à esquerda (faz uma permutação cíclica)
// usada na expansão da chave
void rotWord(char *word) {

    char first_byte = word[0];
    for (char i = 0; i < 3; i++) {
        word[i] = word[i+1];
    }
    word[3] = first_byte;
}

// substitui cada um dos 4 bytes de uma palavra usando one time pad
// precisa de uma chave própria para o one time pad
// usada na expansão da chave
void subWord(char *word, char* otp_key) {

    char *textoCifrado = otpEncryption(word, otp_key, 4);
    for (char i = 0; i < 4; i++) {
        word[i] = textoCifrado[i];
    }
    free(textoCifrado);

}

// atualiza valor da constante de rodada R[i], usada na expansão da chave
// R[] deve ter sido inicializado com {, 0x00, 0x00, 0x00}
void RCon_1(char i, char R[4]) {

    R[0] = 0x01;
    if (i > 1) {
        R[0] = 0x02;
        while (i > 2) {
            R[0] = byteMult(R[0], 0x02);
            i--;
        }
    }
}

// tabela de valores pré-calculados para RCon (até 14 rodadas)
static const char RConTableable[] = {
    0x00,  // RCon[0] não é usado
    0x01,  // x^(1-1) = 0000 0001
    0x02,  // x^(2-1) = 0000 0010
    0x04,  // x^(3-1) = 0000 0100
    0x08,  // x^(4-1) = 0000 1000
    0x10,  // x^(5-1) = 0001 0000
    0x20,  // x^(6-1) = 0010 0000
    0x40,  // x^(7-1) = 0100 0000
    0x80,  // x^(8-1) = 1000 0000
    0x1B,  // x^(9-1) = 1 0000 0000 ^ 1 0001 1011 = 0001 1011
    0x36,  // x^(10-1) = 0011 0110
    0x6C,  // x^(11-1) = 0110 1100
    0xD8,  // x^(12-1) = 1101 1000
    0xAB,  // x^(13-1) = 1 1011 0000 ^ 1 0001 1011 = 1010 1011
    0x4D   // x^(14-1) = 1 0101 0110 ^ 1 0001 1011 = 0100 1101
};

// atualiza valor da constante de rodada R[i]
void RCon(char i, char R[4]) {

    R[0] = RConTable[i];
    R[1] = 0x00;
    R[2] = 0x00; 
    R[3] = 0x00;
}

// verifica se valor da rodada é válido
/*char* RCon(char i) {
    static char R[4] = {0x00, 0x00, 0x00, 0x00};
    if (i < sizeof(RConTable))
        R[0] = RConTable[i];
    return R;
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// expande a chave para Nr rodadas além da adição inicial,
// totalizando Nb*(Nr+1) palavras
// w recebe a chave expandida
void keyExpansion(char *key, char *w, char* otp_key) {

    char temp_word[4], R[4];
    char w_length = NB*(NR+1);

    for (char i = 0; i < 4*NK; i++) {
        w[i] = key[i];
    }

    for (char i = NK; i < w_length; i++) {
        char *current_word = &w[4*i];
        char *prev_word = &w[4*(i-1)];
        char *nk_prev_word = &w[4*(i-NK)];

        temp_word[0] = prev_word[0];
        temp_word[1] = prev_word[1];
        temp_word[2] = prev_word[2];
        temp_word[3] = prev_word[3];

        if (i%NK == 0) {
            rotWord(temp_word);
            subWord(temp_word, otp_key);
            RCon(i/NK, R);
            temp_word[0] ^= R[0];
            temp_word[1] ^= R[1];
            temp_word[2] ^= R[2];
            temp_word[3] ^= R[3];
//        } else if (NK > 6 && i%NK == 4) {
//            subWord(temp_word, otp_key);
        }

        current_word[0] = nk_prev_word[0] ^ temp_word[0];
        current_word[1] = nk_prev_word[1] ^ temp_word[1];
        current_word[2] = nk_prev_word[2] ^ temp_word[2];
        current_word[3] = nk_prev_word[3] ^ temp_word[3];
    }
}

// adiciona a chave da rodada à matriz de estado,
// cada uma das NB colunas da matriz é somada a uma palavra da chave
// colunas e palavras são de 4 bytes
void addRoundKey(char *state, char *w, char round) {

    for (char c = 0; c < NB ; c++) {
        state[NB*0+c] = state[NB*0+c]^w[4*(round*NB+c)+0];
        state[NB*1+c] = state[NB*1+c]^w[4*(round*NB+c)+1];
        state[NB*2+c] = state[NB*2+c]^w[4*(round*NB+c)+2];
        state[NB*3+c] = state[NB*3+c]^w[4*(round*NB+c)+3]; // 4 = tam palavra, round*NB+c = índice da palavra, 3 = índice do byte dentro da palavra
        }
}

// substitui cada byte da matriz de estado usando one time pad
// precisa de uma chave gerada aleatoriamente
void subBytes(char *state, char *otp_key) {

    char *textoCifrado = otpEncryption(state, otp_key, 4*NB);
    for (char i = 0; i < 4; i++) {
        for (char j = 0; j < NB; j++) {
            state[NB*i+j] = textoCifrado[NB*i*j];
        }
    }
    free(textoCifrado);
}

// substitui cada byte da matriz de estado usando one time pad
// precisa da chave usada na cifra
void invSubBytes(char *state, char *otp_key) {

    char *textoDecifrado = otpDecryption(state, otp_key, 4*NB);
    for (char i = 0; i < 4; i++) {
        for (char j = 0; j < NB; j++) {
            state[NB*i+j] = textoDecifrado[NB*i*j];
        }
    }
    free(textoDecifrado);
}

// desloca cada linha r da matriz de estado, r posições à esquerda 
void shiftRows(char *state) {

    char buffer[4];

    for (char r = 1; r < 4; r++) {
        for (char c = 0; c < NB; c++) {
            buffer[c] = state[NB*r+((c+r)%NB)];
        }
        for (char c = 0; c < NB; c++) {
            state[NB*r+c] = buffer[c];
        }
    }

}

// desloca cada linha r da matriz de estado, r posições à direita 
void invShiftRows(char *state) {

    char buffer[4];

    for (char r = 1; r < 4; r++) {
        for (char c = 0; c < NB; c++) {
            buffer[(c+r)%NB] = state[NB*r+c];
        }
        for (char c = 0; c < NB; c++) {
            state[NB*r+c] = buffer[c];
        }
    }

}

// multiplica cada coluna da matriz de estado por a(x)
// a(x) = {03}x3 + {01}x2 + {01}x1 + {02}
void mixColumns_1(char *state) {

    char a[] = {0x02, 0x01, 0x01, 0x03};
    char column[4], result[4];

    for (char j = 0; j < NB; j++) {
        for (char i = 0; i < 4; i++) {
            column[i] = state[NB*i+j];
        }

        word_mult(a, column, result);

        for (char i = 0; i < 4; i++) {
            state[NB*i+j] = result[i];
        }
    }
}

// multiplica cada coluna da matriz de estado por a(x) em GF(256)
// cada coluna de 4 bytes é vista como um polinômio de 4 termos, cada byte é um coeficiente do polinômio 
// módulo polinomio irredutível m(x) = x4 + 1
// a(x) = {03}x3 + {01}x2 + {01}x1 + {02}
void mixColumns(char *state) {

    for (char j = 0; j < NB; j++) {

        char s0 = state[NB*0+j];
        char s1 = state[NB*1+j];
        char s2 = state[NB*2+j];
        char s3 = state[NB*3+j];

        state[NB*0+j] = byteMult(0x02, s0) ^ byteMult(0x03, s1) ^ s2                 ^ s3;
        state[NB*1+j] = s0                 ^ byteMult(0x02, s1) ^ byteMult(0x03, s2) ^ s3;
        state[NB*2+j] = s0                 ^ s1                 ^ byteMult(0x02, s2) ^ byteMult(0x03, s3);
        state[NB*3+j] = byteMult(0x03, s0) ^ s1                 ^ s2                 ^ byteMult(0x02, s3);
    }
}

// multiplica cada coluna da matriz de estado por a-1(x) em GF(256)
// cada coluna de 4 bytes é vista como um polinômio de 4 termos, cada byte é um coeficiente do polinômio 
// módulo polinomio irredutível m(x) = x4 + 1
// a-1(x) = {0b}x3 + {0d}x2 + {09}x1 + {0e}
void invMixColumns(char *state) {

    for (char j = 0; j < NB; j++) {

        char s0 = state[NB*0+j];
        char s1 = state[NB*1+j];
        char s2 = state[NB*2+j];
        char s3 = state[NB*3+j];

        state[NB*0+j] = byteMult(0x0e, s0) ^ byteMult(0x0b, s1) ^ byteMult(0x0d, s2) ^ byteMult(0x09, s3);
        state[NB*1+j] = byteMult(0x09, s0) ^ byteMult(0x0e, s1) ^ byteMult(0x0b, s2) ^ byteMult(0x0d, s3);
        state[NB*2+j] = byteMult(0x0d, s0) ^ byteMult(0x09, s1) ^ byteMult(0x0e, s2) ^ byteMult(0x0b, s3);
        state[NB*3+j] = byteMult(0x0b, s0) ^ byteMult(0x0d, s1) ^ byteMult(0x09, s2) ^ byteMult(0x0e, s3);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// cifra 16 bytes = 128 bits com aes usando one time pad como substituição
// in, out e otp_key deve ter tamanho 4*NB bytes
void aesCipher(char* in, char* out, char *w, char* otp_key) {

    char* state = malloc(4*NB * sizeof(char));

    for (char i = 0; i < 4; i++) {
        for (char j = 0; j < NB; j++) {
            state[NB*i+j] = in[i+4*j];
        }
    }

    addRoundKey(state, w, 0);

    for (char r = 1; r < NR; r++) {
        subBytes(state, otp_key);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, w, r);
    }

    subBytes(state, otp_key);
    shiftRows(state);
    addRoundKey(state, w, NR);

    for (char i = 0; i < 4; i++) {
        for (char j = 0; j < NB; j++) {
            out[i+4*j] = state[NB*i+j];
        }
    }

    free(state);
}

void aesInvCipher(char* in, char* out, char *w, char* otp_key) {

    char* state = malloc(4*NB * sizeof(char));

    for (char i = 0; i < 4; i++) {
        for (char j = 0; j < NB; j++) {
            state[NB*i+j] = in[i+4*j];
        }
    }

    addRoundKey(state, w, NR);

    for (char r = NR-1; r >= 1; r--) {
        invShiftRows(state);
        invSubBytes(state, otp_key);
        addRoundKey(state, w, r);
        invMixColumns(state);
    }

    invShiftRows(state);
    invSubBytes(state, otp_key);
    addRoundKey(state, w, 0);

    for (char i = 0; i < 4; i++) {
        for (char j = 0; j < NB; j++) {
            out[i+4*j] = state[NB*i+j];
        }
    }

    free(state);
}