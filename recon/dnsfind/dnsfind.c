#include "../../lib/dnslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512

int checkDomain(const char *domainName) {
    char ipbuffer[1024];
    if (resolveHostname(domainName, ipbuffer, sizeof(ipbuffer)) == 0) {
        printf("[+] Found domain %s -> %s\n", domainName, ipbuffer);
        return 0;
    }
    return -1;
}

void findSubdomains(const char *domainName, const char *wordlistName) {
    FILE *wordlistFile = fopen(wordlistName, "r");
    if (!wordlistFile) {
        perror("wordlist");
        exit(1);
    }

    char lineBuffer[MAX_LINE];
    while (fgets(lineBuffer, sizeof(lineBuffer), wordlistFile) != NULL) {
        // strip newline
        lineBuffer[strcspn(lineBuffer, "\r\n")] = 0;

        char domainAttempt[512];
        snprintf(domainAttempt, sizeof(domainAttempt), "%s.%s", lineBuffer, domainName);

        checkDomain(domainAttempt);
    }

    fclose(wordlistFile);
}

int main(int argc, char *argv[]) {
    printf("[*] Starting DNS Finder 1.0");
    if (argc < 3) {
        printf("Usage: ./dnsfind <Domain> <Wordlist>\n", argv[0]);
        return 1;
    }

    const char *domainName = argv[1];
    const char *wordlistName = argv[2];

    findSubdomains(domainName, wordlistName);

    return 0;
}
