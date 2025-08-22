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
#ifndef ftplib_h
#define ftplib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// FTP connection structure
typedef struct {
    CURL *curl;
    char *urlBase;
} FTPConnection;

// Response structure for FTP operations
typedef struct {
    char *data;
    size_t size;
} FTPResponse;

// Core FTP functions for brute forcing
FTPConnection* ftpConnect(const char *host, int port, const char *username, const char *password);
void ftpDisconnect(FTPConnection *ftpObject);

// Helper functions
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, FTPResponse *response);
static void initFTPResponse(FTPResponse *response);
static void freeFTPResponse(FTPResponse *response);
static void resetCurlState(CURL *curl);

// FTP connection function - tests credentials
FTPConnection* ftpConnect(const char *host, int port, const char *username, const char *password) {
    if (!host || !username || !password) {
        return NULL;
    }

    FTPConnection *ftp = malloc(sizeof(FTPConnection));
    if (!ftp) {
        return NULL;
    }

    ftp->curl = curl_easy_init();
    if (!ftp->curl) {
        free(ftp);
        return NULL;
    }

    // Build URL
    ftp->urlBase = malloc(strlen(host) + 32);
    snprintf(ftp->urlBase, strlen(host) + 32, "ftp://%s:%d/", host, port);

    // Build credentials
    char userpass[256];
    snprintf(userpass, sizeof(userpass), "%s:%s", username, password);

    // Configure CURL for connection test
    curl_easy_setopt(ftp->curl, CURLOPT_URL, ftp->urlBase);
    curl_easy_setopt(ftp->curl, CURLOPT_USERPWD, userpass);
    curl_easy_setopt(ftp->curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(ftp->curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(ftp->curl, CURLOPT_NOBODY, 1L);

    // Test the connection
    CURLcode result = curl_easy_perform(ftp->curl);
    
    if (result != CURLE_OK) {
        ftpDisconnect(ftp);
        return NULL;
    }

    return ftp;
}

// Disconnect and cleanup
void ftpDisconnect(FTPConnection *ftpObject) {
    if (ftpObject) {
        if (ftpObject->curl) {
            curl_easy_cleanup(ftpObject->curl);
        }
        if (ftpObject->urlBase) {
            free(ftpObject->urlBase);
        }
        free(ftpObject);
    }
}

// Helper functions (keeping the same structure as original)
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, FTPResponse *response) {
    size_t realsize = size * nmemb;
    char *ptr = realloc(response->data, response->size + realsize + 1);
    if (!ptr) {
        return 0;
    }
    
    response->data = ptr;
    memcpy(&(response->data[response->size]), contents, realsize);
    response->size += realsize;
    response->data[response->size] = 0;
    
    return realsize;
}

static void initFTPResponse(FTPResponse *response) {
    response->data = malloc(1);
    response->size = 0;
    if (response->data) {
        response->data[0] = '\0';
    }
}

static void freeFTPResponse(FTPResponse *response) {
    if (response->data) {
        free(response->data);
        response->data = NULL;
    }
    response->size = 0;
}

static void resetCurlState(CURL *curl) {
    if (curl) {
        curl_easy_reset(curl);
    }
}

#endif
