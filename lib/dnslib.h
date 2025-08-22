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
