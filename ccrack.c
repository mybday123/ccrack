#include "ccrack.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

void to_hex(const unsigned char *digest, int len, char *out_hex) {
    for (int i = 0; i < len; i++) {
        sprintf(out_hex + (i * 2), "%02x", digest[i]);
    }
    out_hex[len * 2] = '\0';
}

void hash_md5(const char *input, char *out_hex) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((const unsigned char*)input, strlen(input), digest);
    to_hex(digest, MD5_DIGEST_LENGTH, out_hex);
}

void hash_sha1(const char *input, char *out_hex) {
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1((const unsigned char*)input, strlen(input), digest);
    to_hex(digest, SHA_DIGEST_LENGTH, out_hex);
}

void hash_sha256(const char *input, char *out_hex) {

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)input, strlen(input), digest);
    to_hex(digest, SHA256_DIGEST_LENGTH, out_hex);
}

char **load_wordlist(const char *filename, size_t *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    size_t cap = 1024;
    size_t n = 0;
    char **words = malloc(cap * sizeof(char *));
    if (!words) {
        fclose(fp);
        return NULL;
    }

    char buf[WORD_MAX];
    while (fgets(buf, sizeof(buf), fp)) {
        buf[strcspn(buf, "\r\n")] = 0;
        if (n >= cap) {
            cap *= 2;
            char **tmp = realloc(words, cap * sizeof(char *));
            if (!tmp) {
                for (size_t i = 0; i < n; ++i) free(words[i]);
                free(words);
                fclose(fp);
                return NULL;
            }
            words = tmp;
        }
        words[n] = strdup(buf);
        if (!words[n]) {
            for (size_t i = 0; i < n; ++i) free(words[i]);
            free(words);
            fclose(fp);
            return NULL;
        }
        n++;
    }
    fclose(fp);
    *count = n;
    return words;
}

void *crack_worker(void *arg) {
    struct CrackThreadArgs *args = (struct CrackThreadArgs *)arg;
    char hashed[HASH_HEX_MAX];

    for (size_t i = args->start; i < args->end && !*(args->found); ++i) {
        const char *word = args->words[i];
        switch (args->hashtype) {
            case HASH_MD5:
                hash_md5(word, hashed);
                break;
            case HASH_SHA1:
                hash_sha1(word, hashed);
                break;
            case HASH_SHA256:
                hash_sha256(word, hashed);
                break;
            default:
                continue;
        }
        if (strcmp(hashed, args->target_hash) == 0) {
            pthread_mutex_lock(args->result_mutex);
            if (!*(args->found)) {
                *(args->found) = 1;
                strncpy(args->result_word, word, WORD_MAX-1);
                args->result_word[WORD_MAX-1] = '\0';
            }
            pthread_mutex_unlock(args->result_mutex);
            break;
        }
    }
    return NULL;
}
