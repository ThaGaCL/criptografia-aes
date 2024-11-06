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

char *encryption(char * cltext, char * key, int key_size){
    char *encrypted = malloc(sizeof(char) * (key_size + 1));
    int *cipher = malloc(sizeof(int) * (key_size));

    for(int i = 0; i < key_size; i++){
        cipher[i] = (cltext[i] - 'A') + (key[i] - 'A');
    }

    for (int i = 0; i < key_size; i++) {
        if (cipher[i] > 25) {
            cipher[i] -= 26;
        }
    }

    for (int i = 0; i < key_size; i++) {
        encrypted[i] = cipher[i] + 'A';
    }

    encrypted[key_size] = '\0';

    free(cipher);

    return encrypted;
}

char *decryption(char *cphtext, char *key, int key_size) {
    char *decrypted = malloc(sizeof(char) * (key_size + 1));
    int *plain = malloc(sizeof(int) * key_size);

    // Calcula os valores decifrados subtraindo a chave do texto cifrado
    for (int i = 0; i < key_size; i++) {
        plain[i] = (cphtext[i] - 'A') - (key[i] - 'A');
    }

    // Ajusta valores negativos para o intervalo [0, 25]
    for (int i = 0; i < key_size; i++) {
        if (plain[i] < 0) {
            plain[i] += 26;
        }
    }

    // Converte os valores decifrados em caracteres e armazena na string decifrada
    for (int i = 0; i < key_size; i++) {
        decrypted[i] = plain[i] + 'A';
    }

    decrypted[key_size] = '\0';

    free(plain);

    return decrypted;
}

int calcStringSize(char *str){
    return strlen(str);
}

char *generateRandKey(int key_size) {
    char *randkey = malloc(sizeof(char) * (key_size + 1));  // Aloca espaço extra para o caractere nulo
    if (randkey == NULL) {  // Verifica se a alocação foi bem-sucedida
        return NULL;
    }

    for (int i = 0; i < key_size; i++) {
        randkey[i] = (rand() % 26) + 'A';  // Gera um caractere aleatório de 'A' a 'Z'
    }

    randkey[key_size] = '\0';  // Adiciona o caractere nulo no final da string

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

