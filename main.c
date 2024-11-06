#include "otp/otp.h"
#include "aes/aes.h"
#include <time.h>

int main(int argc, char* argv[])
{
    srand(0);
    

    if(argc < 2){
        printf("Uso correto ./opt <caminho/para/entrada.txt>");
        return 0;
    }

    clock_t time_req;
    const char *caminho = argv[1];
    char *textoClaro = readEntry(caminho);
    /// SETUP PARA O ONE TIME PAD
    int key_size = calcStringSize(textoClaro);
    char *chave = generateRandKey(key_size);    
    convertToUpper(textoClaro, key_size);
    convertToUpper(chave, key_size);


    // DEPOIS ESSAS FUNÇÕES VÃO SER CHAMADAS PELO AES
    // ENCRIPTA COM OTP
    time_req = clock();
    char *textoEncriptado = encryption(textoClaro, chave, key_size);
    time_req = clock() - time_req;
    printf("Tempo gasto encriptando: %f\n",(float)time_req / CLOCKS_PER_SEC);

    // DECIFRA COM OTP
    time_req = clock();
    char *textoDecifrado = decryption(textoEncriptado, chave, key_size);
    time_req = clock() - time_req;
    printf("Tempo gasto decifrando: %f\n",(float)time_req / CLOCKS_PER_SEC);


    return 0;
}