#include <stdio.h>
#include "aes.h"

int main () {

    int key;

    char* w = malloc(NB*(NR+1)*4);
    key_expansion(key, w, otp_key);

    aes_cipher(in , out, w, otp_key);

    printf("texto cifrado:\n");
    for (char i = 0; i < 4; i++) {
        printf("%02x %02x %02x %02x ", out[4*i+0], out[4*i+1], out[4*i+2], out[4*i+3]);
    }
    printf("\n");

    aes_inv_cipher(out, in, w, otp_key);

    printf("texto claro após decifrar:\n");
    for (char i = 0; i < 4; i++) {
        printf("%02x %02x %02x %02x ", in[4*i+0], in[4*i+1], in[4*i+2], in[4*i+3]);
    }
    printf("\n");

    free(w);

    return 0;
}