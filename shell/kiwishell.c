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
  if (!mainWord)
    return;

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
    snprintf(cmd, sizeof(cmd), "../recon/portscanner/portscanner %s %s",
             targetIP, scanFile);

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
    snprintf(cmd, sizeof(cmd), "../recon/dnsfind/dnsfind %s %s", domain,
             wordlist);

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
    snprintf(cmd, sizeof(cmd), "../recon/webfind/webfind %s %s", targetURL,
             wordlist);

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
      printf("Usage: webdrop <IP> <Local file to upload> <FTP username> <FTP "
             "password>\n");
      return;
    }

    // Build the command
    char cmd[256];
    snprintf(cmd, sizeof(cmd),
             "python3 ../exploit/webdrop/webdrop.py %s %s %s %s\n", targetIP,
             localFile, username, password);

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
  // Command for SQLi payloads
  else if (strcmp(mainWord, "sqli") == 0) {
    // Print the markdown file of payloads
    system(
        "echo -n '\n' && cat ../payload/sqli/payloadlist.md && echo -n '\n'");
    return;
  }
  // Reverse shell creator tool
  else if (strcmp(mainWord, "rshell") == 0) {
    // Run the python reverse shell generator
    system("python3 ../payload/rshell/shellgen.py");
  }
  // Bruteclaw command
  else if (strcmp(mainWord, "bruteclaw") == 0) {
    // Get arguments
    char *targetIP = strtok(NULL, " ");
    char *usernameFile = strtok(NULL, " ");
    char *passwordFile = strtok(NULL, " ");
    char *flag = strtok(NULL, " ");

    // Verify tokens
    if (!targetIP || !usernameFile || !passwordFile || !flag) {
      printf(
          "Usage: bruteclaw <IP> <Username file> <Password file> <ftp|ssh>\n");
      return;
    }
    // Build the command
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "../utility/bruteclaw/bruteclaw %s %s %s %s",
             targetIP, usernameFile, passwordFile, flag);

    // Run the commmand
    system(cmd);
  }
  // DNS tool command
  else if (strcmp(mainWord, "dnstool") == 0) {
    // Get tokens
    char *information = strtok(NULL, " ");
    char *targetInformation = strtok(NULL, " ");

    // Verify tokens
    if (!information || !targetInformation) {
      printf("Usage: dnstool <IP|Domain> <resolve|reverse>\n");
      return;
    }

    // Build the command
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "../utility/dnstool/dnstool %s %s", information,
             targetInformation);

    // Run the command
    system(cmd);
  }
  // SSH client command
  else if (strcmp(mainWord, "sshclient") == 0) {
    // Get tokens
    char *targetIP = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *password = strtok(NULL, " ");

    // Verify tokens
    if (!targetIP || !username || !password) {
      printf("Usage: sshclient <IP> <Username> <Password>\n");
      return;
    }

    // Build command
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "../utility/sshclient/sshclient %s %s %s",
             targetIP, username, password);

    // Run the command
    system(cmd);
  }

  // NetCatcher command
  else if (strcmp(mainWord, "netcatcher") == 0) {
    // Get tokens
    char *packetCount = strtok(NULL, " ");
    char *filter = strtok(NULL, " ");

    // Verify tokens
    if (!packetCount || !filter) {
      printf("Usage: netcatcher <Packet count> <Filter>\n");
      return;
    }

    // Build command
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "../utility/netcatcher/netcatcher %s %s",
             packetCount, filter);

    // Run the command
    system(cmd);
  }

  // Command not found
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
int main(int argc, char *argv[]) {
  // Check if any command line arguments where used
  if (argc >= 2 &&
      (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)) {
    printf("Kiwi shell 1.1\n");
    return 0;
  } else if (argc >= 2 &&
             (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
    printf("Launch kiwi: ./kiwishell\nCompile all tools: bash "
           "build.sh\nDocumentation for each tool in README\n");
    return 0;
  }

  homeFunction();    // Print banner and commands list
  mainCommandLoop(); // Start the main command loop
  return 0;
}
