// Portscanner.c
#include "../../lib/configlib.h"
#include "../../lib/socketlib.h"
#include <stdio.h>
#include <stdlib.h>

// Flag data
#define LOGGING_FLAG 1
#define VERBOSETEXT_FLAG 2
#define VERBOSELOG_FLAG 4
#define LONGTIMEOUT_FLAG 8

// Return 1 for open port, 0 for closed
int checkPort(const char *targetIP, int port, int timeoutSeconds) {
  int socketFD = TCPClientConnect(targetIP, port, timeoutSeconds);
  if (socketFD == -1) {
    return 0;
  }
  printf("[+] Port %d is open\n", port);
  TCPClose(socketFD);
  return 1;
}

// Loop through a list of ports
void scanIP(const char *targetIP, int portList[], int portCount, int flag) {
  int timeoutSeconds = 1;
  int loggingLevel =
      0; // 0 = no logging, 1 = log open ports, 2 = log open and closed
  int verboseText =
      0; // 0 only show open ports, 1 show state of all scanned ports

  // Check each flag
  if (flag & LONGTIMEOUT_FLAG)
    timeoutSeconds =
        3; // Increase timeout if longtimeout was set for slower networks
  if (flag & VERBOSETEXT_FLAG)
    verboseText = 1; // Print state of all ports
  if (flag & LOGGING_FLAG)
    loggingLevel = 1; // Log open ports to a file
  if (flag & VERBOSELOG_FLAG)
    loggingLevel = 2; // Log open and closed ports to a file

  FILE *loggingFile = NULL;
  if (loggingLevel > 0) {
    loggingFile = fopen("scanlog.txt", "w");
    if (loggingFile == NULL) {
      perror("fopen");
      return;
    }
    fprintf(loggingFile, "%s\n", targetIP);
  }

  // Loop over ports
  for (int i = 0; i < portCount; i++) {
    if (verboseText) {
      printf("[*] Scanning port %d\n", portList[i]);
    }

    int portOpen = checkPort(targetIP, portList[i], timeoutSeconds);

    if (loggingFile) {
      if (loggingLevel == 1 && portOpen == 1) {
        fprintf(loggingFile, "%d open\n", portList[i]);
      }
      if (loggingLevel == 2) {
        fprintf(loggingFile, "%d %s\n", portList[i],
                portOpen ? "open" : "closed");
      }
    }
  }

  // If logging was enabled, then close the logging file
  if (loggingFile) {
    fclose(loggingFile);
  }
}

int main(int argc, char *argv[]) {
  printf("[*] Starting Portscanner 1.2\n");

  if (argc < 3) {
    printf("Usage: %s <IP> <Scan file>\n", argv[0]);
    return 1;
  }

  // Get CLI arguments
  const char *targetIP = argv[1];
  const char *scanfileFilename = argv[2];

  configFileData scanfileData = loadConfigFile(scanfileFilename);

  // Null check in case the scanfile can't be found
  if (!scanfileData.intArray || scanfileData.intCount == 0) {
    fprintf(stderr, "Invalid scan file or no ports to scan.\n");
    return 1;
  }

  // Convert the structure returned by config lib into a set of variables
  int flag = scanfileData.header;
  int *portList = scanfileData.intArray;
  int portCount = scanfileData.intCount;

  scanIP(targetIP, portList, portCount, flag);

  freeConfig(&scanfileData);
  return 0;
}
