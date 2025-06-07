#include "ccrack.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <hash> <wordlist>\n", argv[0]);
        exit(1);
    }

    const char *target_hash = argv[1];
    const char *wordlist_path = argv[2];
    size_t hash_len = strlen(target_hash);
    enum HashType hashtype;

    if (hash_len == MD5_LEN) {
        puts("Using MD5");
        hashtype = HASH_MD5;
    } else if (hash_len == SHA1_LEN) {
        puts("Using SHA1");
        hashtype = HASH_SHA1;
    } else if (hash_len == SHA256_LEN) {
        puts("Using SHA256");
        hashtype = HASH_SHA256;
    } else {
        fprintf(stderr, "Unsupported length: %zu", hash_len);
        exit(1);
    }

    FILE *wordlistptr = fopen(wordlist_path, "r");
    if (!wordlistptr) {
        perror("Cannot open wordlist");
        exit(1);
    }

    char word[WORD_MAX];
    char hashed[HASH_HEX_MAX];
    bool found = false;

    while (fgets(word, WORD_MAX, wordlistptr)) {
        word[strcspn(word, "\r\n")] = 0;

        switch (hashtype) {
            case HASH_MD5:
                hash_md5(word, hashed);
                printf("hashed: %s\ntarget_hash: %s\n", hashed, target_hash);
                break;
            case HASH_SHA1:
                hash_sha1(word, hashed);
                break;
            case HASH_SHA256:
                hash_sha256(word, hashed);
                break;
            default:
                break;
        }

        if(strcmp(hashed, target_hash) == 0) {
            printf("Match found: '%s'\n", word);
            found = true;
            break;
        }
    }

    fclose(wordlistptr);
    if (!found) {
        puts("No match found in wordlist :(");
    }
    return 0;
}
