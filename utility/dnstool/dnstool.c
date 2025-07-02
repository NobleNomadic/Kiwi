// Dnstool.c
#include "../../lib/dnslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Standard DNS resolve
void convertHostToIP(const char *hostname) {
    char foundIP[1024];
    int resolveSuccess = resolveHostname(hostname, foundIP, sizeof(foundIP));

    if (resolveSuccess == 0) {
        printf("[+] IP of %s: %s\n", hostname, foundIP);        
        return;
    }

    printf("[-] Unable to find IP of %s\n", hostname);
    return;
}

// Reverse DNS resolve to find domain
void convertIPToDomain(const char *targetIP) {
    char foundDomain[1024];
    int reverseSuccess = resolveIP(targetIP, foundDomain, sizeof(foundDomain));

    if (reverseSuccess == 0) {
        printf("[+] Domain of %s: %s\n", targetIP, foundDomain);
        return;
    }

    printf("[-] Unable to found domain of %s\n", targetIP);
    return;
}

int main(int argc, char *argv[]) {
    printf("[*] Starting DNS Tool 1.0\n");
    if (argc < 3) {
        printf("Usage: %s <IP|Domain> <resolve|reverse>\n", argv[0]);
    }

    const char *information = argv[1];
    const char *targetInformation = argv[2];

    if (strcmp(targetInformation, "resolve") == 0) {
        convertHostToIP(information);
    }

    else if (strcmp(targetInformation, "reverse") == 0) {
        convertIPToDomain(information);
    }
}
