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
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512
#define MAX_URL_LEN 2048

// Function to check the HTTP status code for a given URL
int checkUrl(const char *url) {
    CURL *curl;
    CURLcode res;
    long http_code = 0;

    // Initialize CURL session
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "CURL initialization failed!\n");
        return -1;
    }

    // Set the URL to check
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);  // Set timeout to 10 seconds

    // Perform the HTTP request
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return -1;
    }

    // Get the HTTP status code
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    // Handle the status codes
    if (http_code == 200) {
        printf("[+] Found: %s (200 OK)\n", url);
    } else if (http_code == 301 || http_code == 302) {
        printf("[+] Found: %s (Redirect)\n", url);
    } else if (http_code == 403) {
        printf("[+] Found: %s (403 Forbidden)\n", url);
    } else {
        // Ignore 404 and other non-useful status codes for now
        curl_easy_cleanup(curl);
        return -1;
    }

    // Cleanup and return
    curl_easy_cleanup(curl);
    return 0;
}

// Function to perform the directory/file brute-forcing
void findDirectoriesAndFiles(const char *targetUrl, const char *wordlistName) {
    FILE *wordlistFile = fopen(wordlistName, "r");
    if (!wordlistFile) {
        perror("wordlist");
        exit(1);
    }

    char lineBuffer[MAX_LINE];
    while (fgets(lineBuffer, sizeof(lineBuffer), wordlistFile) != NULL) {
        // Strip newline
        lineBuffer[strcspn(lineBuffer, "\r\n")] = 0;

        char url[MAX_URL_LEN];
        snprintf(url, sizeof(url), "%s/%s", targetUrl, lineBuffer);  // Construct the full URL

        // Check the URL
        checkUrl(url);
    }

    fclose(wordlistFile);
}

int main(int argc, char *argv[]) {
    printf("[*] Starting Directory/File Brute Forcer 1.0\n");

    if (argc < 3) {
        printf("Usage: %s <Target URL> <Wordlist>\n", argv[0]);
        return 1;
    }

    const char *targetUrl = argv[1];
    const char *wordlistName = argv[2];

    // Start brute-forcing directories and files
    findDirectoriesAndFiles(targetUrl, wordlistName);

    return 0;
}
