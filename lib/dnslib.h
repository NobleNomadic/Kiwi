#ifndef dnslib_h
#define dnslib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

// Resolve a hostname. Get the IP from domain
int resolveHostname(const char *hostname, char *ipbuf, size_t buflen) {
    // Setup address structure
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Check status of host
    int status = getaddrinfo(hostname, NULL, &hints, &res);
    if (status != 0) return -1;

    // Write the IP to the buffer
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipbuf, buflen);

    // Clean up and return success
    freeaddrinfo(res);
    return 0;
}

// Reverse resolve a hostname. Get domain from IP
int resolveIP(const char *targetIP, char *hostbuf, size_t buflen) {
    // Setup address structure
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, targetIP, &(sa.sin_addr));

    // Get status
    int status = getnameinfo((struct sockaddr *)&sa, sizeof(sa), hostbuf, buflen, NULL, 0, NI_NAMEREQD);
    if (status != 0) {
        return -1;
    }
    return 0;
}

#endif
