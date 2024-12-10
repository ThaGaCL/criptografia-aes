#include "otp/otp.h"
#include "aes/aes.h"
#include <time.h>
#include <locale.h>

int main(int argc, char* argv[])
{
    srand(0);
    setlocale(LC_ALL,"Portuguese");

    if(argc < 3){
        printf("Uso correto ./opt <caminho/para/entrada.txt> <chave>\n");
        return 0;
    }

    clock_t time_req;
    const char *caminho = argv[1];
    const char *chave = argv[2];
    char *textoClaro = readEntry(caminho);
    int key_size = calcStringSize(textoClaro);
    char *textoCifrado = malloc(sizeof(char) * (key_size + 1));
    textoCifrado[key_size] = '\0';

    char *otp_key = generateRandKey(key_size);    
    // convertToUpper(textoClaro, key_size);
    // convertToUpper(otp_key, key_size);

    char in[16];
    char out[16];
    char cur_otp_key[16];
    char* w = malloc(NB*(NR+1)*4);

    // ENCRIPTA COM AES + OTP
    time_req = clock();
    for (size_t block = 0; block < key_size/(NB*4); block++) {
        for (size_t i = 0; i < NB*4; i++) {
            cur_otp_key[i] = otp_key[NB*4*block+i];
            in[i] = textoClaro[NB*4*block+i];
        }
        keyExpansion(chave, w, cur_otp_key);
        aesCipher(in, out, w, cur_otp_key);
        for (size_t i = 0; i < NB*4; i++) {
            textoCifrado[NB*4*block+i] = out[i];
        }
    }
    time_req = clock() - time_req;
    printf("Tempo gasto encriptando: %f\n",(float)time_req / CLOCKS_PER_SEC);

    // DECIFRA COM AES + OTP
    time_req = clock();
    for (size_t block = 0; block < key_size/(NB*4); block++) {
        for (size_t i = 0; i < NB*4; i++) {
            cur_otp_key[i] = otp_key[NB*4*block+i];
            in[i] = textoCifrado[NB*4*block+i];
        }
        keyExpansion(chave, w, cur_otp_key);
        aesInvCipher(in, out, w, cur_otp_key);
        for (size_t i = 0; i < NB*4; i++) {
            textoClaro[NB*4*block+i] = out[i];
        }
    }
    time_req = clock() - time_req;
    printf("Tempo gasto decifrando: %f\n",(float)time_req / CLOCKS_PER_SEC);

    free(textoClaro);
    free(textoCifrado);
    free(otp_key);
    free(w);

    return 0;
}