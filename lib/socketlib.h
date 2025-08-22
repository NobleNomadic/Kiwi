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
#ifndef socketlib_h
#define socketlib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Functions for a TCP server
// Setup and return socket file descriptor
int TCPServerSetup(int port) {
    // Create and setup socket file descriptor
    int socketFD;
    struct sockaddr_in addr;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        perror("socket");
        return -1;
    }

    // Allow the socket to be reused
    int opt = 1;
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Setup the structure for the address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    // Bind the socket
    if (bind(socketFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(socketFD);
        return -1;
    }

    // Start listening
    if (listen(socketFD, 5) < 0) {
        perror("listen");
        close(socketFD);
        return -1;
    }

    // Return the file descriptor of the socket
    return socketFD;
}

// Accept connections using a listening file descriptor
// Once a connection is made, return the file descriptor of the client socket
int TCPServerAccept(int serverFD) {
    // Get client socket structure ready
    int clientFD;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    // Accept connections
    clientFD = accept(serverFD, (struct sockaddr *)&client_addr, &len);
    if (clientFD < 0) {
        perror("accept");
        return -1;
    }

    // Send back the client socket
    return clientFD;
}

// Client connection using TCP socket
// Make connection and return connected socket file descriptor
int TCPClientConnect(const char *targetIP, int port, int timeoutSeconds) {
    // Setup the socket data
    int socketFD;
    struct sockaddr_in addr;

    // Structure for timeout
    struct timeval timeout;

    // Make the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        perror("socket");
        return -1;
    }

    // Add socket timeout
    timeout.tv_sec = timeoutSeconds;
    timeout.tv_usec = 0;

    setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(socketFD, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    // Setup the address structure
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    // Initialise the socket
    if (inet_pton(AF_INET, targetIP, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        return -1;
    }

    // Make the connection
    if (connect(socketFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(socketFD);
        return -1;
    }

    // Return the file descriptor of the connected socket
    return socketFD;
}

// Send data to a socket file descriptor
// Return the number of bytes sent or -1
ssize_t TCPSend(int socketFD, const char *data, size_t len) {
    return send(socketFD, data, len, 0);
}

// Receive data into a buffer from a chosen socket
// Update the buffer that is chosen with received data
// Return the number of bytes received or -1
ssize_t TCPRecv(int socketFD, char *buffer, size_t len) {
    return recv(socketFD, buffer, len, 0);
}

// Close socket and end connection
void TCPClose(int socketFD) {
    close(socketFD);
}

#endif
