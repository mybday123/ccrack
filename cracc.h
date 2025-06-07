#ifndef CCRACK_H
#define CCRACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#define WORD_MAX 256
#define MD5_LEN 32
#define SHA1_LEN 40
#define SHA256_LEN 64
#define HASH_HEX_MAX (SHA256_LEN + 1)

enum HashType {
    HASH_MD5 = 1,
    HASH_SHA1 = 2,
    HASH_SHA256 = 3,
};

void to_hex(const unsigned char *digest, int len, char *out_hex);
void hash_md5(const char *input, char *out_hex);
void hash_sha1(const char *input, char *out_hex);
void hash_sha256(const char *input, char *out_hex);

char **load_wordlist(const char *filename, size_t *count);

struct CrackThreadArgs {
    char **words;
    size_t start;
    size_t end;
    const char *target_hash;
    enum HashType hashtype;
    volatile int *found;
    char *result_word;
    pthread_mutex_t *result_mutex;
};

void *crack_worker(void *arg);

#endif // CCRACK_H
