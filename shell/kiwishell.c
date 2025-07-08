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
