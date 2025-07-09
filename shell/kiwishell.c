// kiwishell.c - Tool for using other Kiwi programs
#include "shellconstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Clears the screen and prints the banner
void homeFunction() {
    system("clear");
    printf("%s", kiwiShellBanner);
    printf("%s", kiwiCommandsMenu);
}

// Process a command string
void processCommand(char *commandString) {
    // Tokenize input
    char *mainWord = strtok(commandString, " ");

    // If empty input
    if (!mainWord) return;

    // Exit command
    if (strcmp(mainWord, "exit") == 0) {
        exit(0);
    }
    // Clear screen and print banner again
    else if (strcmp(mainWord, "reset") == 0 || strcmp(mainWord, "clear") == 0) {
        homeFunction();
        return;
    }
    // Portscanner function
    else if (strcmp(mainWord, "portscanner") == 0) {
        // Get the next 2 tokens
        char *targetIP = strtok(NULL, " ");
        char *scanFile = strtok(NULL, " ");

        // Check if they are valid
        if (!targetIP || !scanFile) {
            printf("Usage: portscanner <IP> <Scan file>\n");
            return;
        }

        // Build the command string
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "../recon/portscanner/portscanner %s %s", targetIP, scanFile);

        // Run the portscanner command
        system(cmd);
        return;
    }
    // DNSFind tool
    else if (strcmp(mainWord, "dnsfind") == 0) {
        // Get next tokens
        char *domain = strtok(NULL, " ");
        char *wordlist = strtok(NULL, " ");

        // Check if tokens are valid
        if (!domain || !wordlist) {
            printf("Usage: dnsfind <Domain> <Wordlist>\n");
            return;
        }

        // Build command to run
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "../recon/dnsfind/dnsfind %s %s", domain, wordlist);

        // Run dnsfind
        system(cmd);
        return;
    }
    // Webfind tool
    else if (strcmp(mainWord, "webfind") == 0) {
        // Get tokens
        char *targetURL = strtok(NULL, " ");
        char *wordlist = strtok(NULL, " ");

        // Check if tokens are valid
        if (!targetURL || !wordlist) {
            printf("Usage: webfind <URL> <Wordlist>\n");
            return;
        }

        // Build the command
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "../recon/webfind/webfind %s %s", targetURL, wordlist);

        // Run the command
        system(cmd);
        return;
    }
    // Webdrop exploit
    else if (strcmp(mainWord, "webdrop") == 0) {
        // Get needed tokens
        char *targetIP = strtok(NULL, " ");
        char *localFile = strtok(NULL, " ");
        char *username = strtok(NULL, " ");
        char *password = strtok(NULL, " ");

        // Check all tokens are valid
        if (!targetIP || !localFile || !username || !password) {
            printf("Usage: webdrop <IP> <Local file to upload> <FTP username> <FTP password>\n");
            return;
        }

        // Build the command
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "python3 ../exploit/webdrop/webdrop.py %s %s %s %s\n", targetIP, localFile, username, password);

        // Run the command
        system(cmd);
        return;
    }
    // Packer exploit scripts
    else if (strcmp(mainWord, "packer") == 0) {
        // Get needed token
        char *packerScript = strtok(NULL, " ");

        // Check if token is valid
        if (!packerScript) {
            printf("Usage: packer <Packer script>\n");
            return;
        }

        // Build the command
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "../exploit/packer/packer %s", packerScript);

        // Run the command
        system(cmd);
        return;
    }
    else {
        printf("Unknown command: %s\n", mainWord);
    }

    return;
}

// Command line loop for input
void mainCommandLoop() {
    char commandBuffer[128];
    while (1) {
        // Get input, then give the input to processCommand()
        printf(">>> ");
        if (fgets(commandBuffer, sizeof(commandBuffer), stdin)) {
            // Remove newline
            commandBuffer[strcspn(commandBuffer, "\n")] = '\0';
            processCommand(commandBuffer);
        }
    }
}

// Entry point
int main() {
    homeFunction();         // Print banner and commands list
    mainCommandLoop();      // Start the main command loop
    return 0;
}
