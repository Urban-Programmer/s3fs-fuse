#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/rc4.h>

char* encrypt(const char* output_path, const char* input_path, const char* key);

int main() {
    char input_path[256], key[256], output_path[256];
    printf("Enter the file path: ");
    fgets(input_path, 256, stdin);
    input_path[strcspn(input_path, "\n")] = 0; // remove newline character
    printf("Enter the key: ");
    fgets(key, 256, stdin);
    key[strcspn(key, "\n")] = 0; // remove newline character
    printf("Enter the output file path: ");
    fgets(output_path, 256, stdin);
    output_path[strcspn(output_path, "\n")] = 0; // remove newline character
    
    char* encrypted_path = encrypt(output_path, input_path, key);
    if (encrypted_path == NULL) {
        printf("Error encrypting file\n");
        return 1;
    }
    
    printf("File successfully encrypted at path: %s\n", encrypted_path);
    return 0;
}

char* encrypt(const char* output_path, const char* input_path, const char* key) {
    FILE* outfile = fopen(output_path, "wb");
    FILE* infile = fopen(input_path, "rb");

    //Error Handling if they're NULL
    if (infile == NULL){
        printf("Cannot open input file\n");
        return NULL;
    }
    if (outfile == NULL){
        printf("Cannot open output file\n");
        return NULL;
    }
    if (key == NULL){
        printf("No Key entered... exiting\n");
        return NULL;
    }

    // Initialize the RC4 key context with the key 
    int key_len = strlen(key);
    RC4_KEY rc4_key;
    RC4_set_key(&rc4_key, key_len, key);

    // Encrypt each byte of the input file using RC4 and write to output file
    unsigned char inbuf;
    unsigned char outbuf;
    size_t bytes_read = fread(&inbuf, 1, sizeof(inbuf), infile);
    while (bytes_read > 0) {
        RC4(&rc4_key, 1, &inbuf, &outbuf);
        fwrite(&outbuf, 1, sizeof(outbuf), outfile);
        bytes_read = fread(&inbuf, 1, sizeof(inbuf), infile);
    }

    // Cleanup
    fclose(infile);
    fclose(outfile);
    
    return strdup(output_path); // return a duplicate of the output_path string
}
