
#include "../../lib/ftplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Interactive FTP client loop
int ftpClient(const char *targetIP, const char *username, const char *password) {
    FTPConnection *ftpObject = ftpConnect(targetIP, 21, username, password);
    if (!ftpObject) {
        printf("[-] FTP setup failure\n");
        return -1;
    }
    printf("[+] Connected to FTP on %s\n", targetIP);

    char input[256];
    while (1) {
        printf("FTP@%s> ", targetIP);
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
            if (ftpRunCommand(ftpObject, input) != 0) {
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
            if (ftpUploadFile(ftpObject, localPath, remotePath) != 0) {
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
            if (ftpDownloadFile(ftpObject, remotePath, localPath) != 0) {
                printf("[-] Download failed\n");
            }
            continue;
        }

        // Unknown command
        printf("[-] Unknown command.\n-- Available: run, upload, download, exit\n");
    }

    ftpDisconnect(ftpObject);
    return 0;
}

// Main function and argument parsing
int main(int argc, char *argv[]) {
    printf("[*] Starting FTP Client 1.0\n");

    if (argc < 4) {
        printf("Usage:./ftpclient <IP> <Username> <Password>\n", argv[0]);
        return 1;
    }

    const char *hostIP = argv[1];
    const char *username = argv[2];
    const char *password = argv[3];

    return ftpClient(hostIP, username, password);
}
