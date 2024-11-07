#include "otp.h"

void convertToUpper(char *text, int size) {
    for (int i = 0; i < size; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = toupper(text[i]);
        }
        // Ignora caracteres fora do intervalo de A-Z
        else if (text[i] < 'A' || text[i] > 'Z') {
            text[i] = ' ';  // substitui caracteres inválidos por espaço (opcional)
        }
    }
}

// Função para criptografar byte a byte com XOR
char *encryption(char *cltext, char *key, int text_size) {
    char *encrypted = malloc(sizeof(char) * (text_size + 1));
    if (encrypted == NULL) {
        perror("Erro ao alocar memória para criptografia");
        return NULL;
    }

    for (int i = 0; i < text_size; i++) {
        encrypted[i] = cltext[i] ^ key[i % text_size];  // XOR byte a byte
    }
    encrypted[text_size] = '\0';

    return encrypted;
}

// Função para descriptografar byte a byte com XOR
char *decryption(char *cphtext, char *key, int text_size) {
    char *decrypted = malloc(sizeof(char) * (text_size + 1));
    if (decrypted == NULL) {
        perror("Erro ao alocar memória para descriptografia");
        return NULL;
    }

    for (int i = 0; i < text_size; i++) {
        decrypted[i] = cphtext[i] ^ key[i % text_size];  // XOR byte a byte
    }
    decrypted[text_size] = '\0';

    return decrypted;
}


int calcStringSize(char *str){
    return strlen(str);
}

// Geração de uma chave aleatória de bytes
char *generateRandKey(int key_size) {
    char *randkey = malloc(sizeof(char) * (key_size + 1));
    if (randkey == NULL) {
        return NULL;
    }

    for (int i = 0; i < key_size; i++) {
        randkey[i] = rand() % 256;  // Gera um byte aleatório (0 a 255)
    }
    randkey[key_size] = '\0';

    return randkey;
}

char *readEntry(const char *caminho) {
    FILE *file = fopen(caminho, "rb");  // Abre o arquivo em modo binário
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *buffer = malloc((fileSize + 1) * sizeof(char));
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0';
    fclose(file);

    return buffer;
}

