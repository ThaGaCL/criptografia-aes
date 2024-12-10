#include "otp.h"
#include <locale.h>

int main(int argc, char* argv[]){
    srand(0);
    setlocale(LC_ALL, "Portuguese");

    if(argc < 2){
        printf("Uso correto ./opt <caminho/para/entrada.txt>");
        return 0;
    }

    const char *caminho = argv[1];
    // printf("Caminho: %s\n", caminho);

    char *textoClaro = readEntry(caminho);

    int key_size = calcStringSize(textoClaro);
    // printf("Tamanho da String: %d\n", key_size);

    char *chave = generateRandKey(key_size);

    // convertToUpper(textoClaro, key_size);
    // convertToUpper(chave, key_size);

    // printf("Texto Claro: %s\nChave Aleatória Gerada: %s\n", textoClaro, chave);


    printf("Encriptando ...\n");
    char *textoEncriptado = otpEncryption(textoClaro, chave, key_size);
    // printf("Texto Cifrado: %s\n", textoEncriptado);

    // printf("Decifrado ...\n");
    char *textoDecifrado = otpDecryption(textoEncriptado, chave, key_size);
    printf("Texto Decifrado: %s\n", textoDecifrado);

    return 0;
}