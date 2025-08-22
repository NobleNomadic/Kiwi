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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Interactive SSH client loop
int sshClient(const char *targetIP, const char *username, const char *password) {
    SSHConnection *sshObject = sshConnect(targetIP, 22, username, password);
    if (!sshObject) {
        printf("[-] SSH setup failure\n");
        return -1;
    }
    printf("[+] Connected to SSH on %s\n", targetIP);

    char input[256];
    while (1) {
        printf("SSH@%s> ", targetIP);
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n[-] Input error or EOF\n");
            break;
        }
        strtok(input, "\n");  // strip newline

        // Exit command
        if (strcmp(input, "exit") == 0) {
            printf("[*] Exiting...\n");
            break;
        }

        // Run remote command
        if (strcmp(input, "run") == 0) {
            printf("Command to run: ");
            if (!fgets(input, sizeof(input), stdin)) {
                printf("\n[-] Input error\n");
                continue;
            }
            strtok(input, "\n");
            if (sshRunCommand(sshObject, input) != 0) {
                printf("[-] Command execution failure\n");
            }
            continue;
        }

        // Upload file
        if (strcmp(input, "upload") == 0) {
            char localPath[256], remotePath[256];
            printf("Local file path: ");
            if (!fgets(localPath, sizeof(localPath), stdin)) {
                printf("\n[-] Input error\n");
                continue;
            }
            strtok(localPath, "\n");
            printf("Remote file path: ");
            if (!fgets(remotePath, sizeof(remotePath), stdin)) {
                printf("\n[-] Input error\n");
                continue;
            }
            strtok(remotePath, "\n");
            if (sshUploadFile(sshObject, localPath, remotePath) != 0) {
                printf("[-] Upload failed\n");
            }
            continue;
        }

        // Download file
        if (strcmp(input, "download") == 0) {
            char remotePath[256], localPath[256];
            printf("Remote file path: ");
            if (!fgets(remotePath, sizeof(remotePath), stdin)) {
                printf("\n[-] Input error\n");
                continue;
            }
            strtok(remotePath, "\n");
            printf("Local file path: ");
            if (!fgets(localPath, sizeof(localPath), stdin)) {
                printf("\n[-] Input error\n");
                continue;
            }
            strtok(localPath, "\n");
            if (sshDownloadFile(sshObject, remotePath, localPath) != 0) {
                printf("[-] Download failed\n");
            }
            continue;
        }

        // Unknown command
        printf("[-] Unknown command.\n-- Available: run, upload, download, exit\n");
    }

    sshDisconnect(sshObject);
    return 0;
}

// Main function and argument parsing
int main(int argc, char *argv[]) {
    printf("[*] Starting SSH Client 1.0\n");

    if (argc < 4) {
        printf("Usage:./sshclient <IP> <Username> <Password>\n", argv[0]);
        return 1;
    }

    const char *hostIP = argv[1];
    const char *username = argv[2];
    const char *password = argv[3];

    return sshClient(hostIP, username, password);
}
