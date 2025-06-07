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
