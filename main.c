#include "cracc.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

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
        fprintf(stderr, "Unsupported length: %zu\n", hash_len);
        exit(1);
    }

    size_t word_count = 0;
    char **words = load_wordlist(wordlist_path, &word_count);
    if (!words) {
        perror("Cannot load wordlist");
        return 1;
    }
    if (word_count == 0) {
        fprintf(stderr, "Wordlist is empty\n");
        return 1;
    }

    long ncores = sysconf(_SC_NPROCESSORS_ONLN);
    int nthreads = (ncores > 1) ? (ncores - 1) : 1;

    pthread_t threads[nthreads];
    struct CrackThreadArgs args[nthreads];
    int found = 0;
    char result_word[WORD_MAX] = {0};
    pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

    size_t chunk = word_count / nthreads;
    size_t rem = word_count % nthreads;
    size_t start = 0;
    for (int i = 0; i < nthreads; ++i) {
        size_t end = start + chunk + (i < rem ? 1 : 0);
        args[i].words = words;
        args[i].start = start;
        args[i].end = end;
        args[i].target_hash = target_hash;
        args[i].hashtype = hashtype;
        args[i].found = &found;
        args[i].result_word = result_word;
        args[i].result_mutex = &result_mutex;
        pthread_create(&threads[i], NULL, crack_worker, &args[i]);
        start = end;
    }

    for (int i = 0; i < nthreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    if (found) {
        printf("Match found: '%s'\n", result_word);
    } else {
        puts("No match found in wordlist :(");
    }

    for (size_t i = 0; i < word_count; ++i) {
        free(words[i]);
    }
    free(words);

    pthread_mutex_destroy(&result_mutex);

    return 0;
}
