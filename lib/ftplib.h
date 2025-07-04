#ifndef ftplib_h
#define ftplib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Structure for a basic FTP connection using curl
typedef struct {
    CURL *curl;
    char *urlBase;
    char *username;
    char *password;
} FTPConnection;

// Create and return an FTP connected object
FTPConnection *ftpConnect(const char *targetIP, const char *username, const char *password) {
    // Create the object used for FTP
    FTPConnection ftpObject = malloc(sizeof(FTPConnection));
    if (!ftpObject) return -1;

    // Create the curl object used for the connection
    ftpObject->curl = curl_easy_init();
    if (!ftpObject->curl) {
        free(FTPConnection);
        return -1;
    }

    // Build the URL for the FTP connection
    size_t len = strlen(targetIP) + 7 + 1; // ftp://host + null
    ftpObject->urlBase = malloc(len);
    snprintf(ftpObject->urlBase, len, "ftp://%s", targetIP);

    // Set the username and password
    // Set values in the object
    ftpObject->username = stdrup(username);
    ftpObject->password = stdrup(password);

    // Set the curl options
    curl_easy_setopt(ftpObject->curl, CURLOPT_USERNAME, ftpObject->username);
    curl_easy_setopt(ftpObject->curl, CURLOPT_PASSWORD, ftpObject->password);

    // Return the object created
    return ftpObject;
}

// Upload a file using a connected object
int ftpUploadFile(FTPConnection *ftpObject, const char *localPath, const char *remotePath) {
    if (!ftpObject || !ftpObject->curl) return -1;

    // Build full URL
    char fullURL[512];
    snprintf(fullURL, sizeof(fullURL), "%s%s", ftpObject->urlBase, remoteFilename);

    // Open the local file
    FILE *localFile = fopen(localPath, "rb");
    if (!localFile) {
        perror("fopen");
        return -1;
    }

    // Build the request
    curl_easy_setopt(ftpObject->curl, CURLOPT_URL, fullURL);
    curl_easy_setopt(ftpObject->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(ftpObject->curl, CURLOPT_READDATA, localFile);

    // Make the request
    CURLcode result = curl_easy_peform(ftpObject->curl);
    fclose(localFile);

    if (result != CURLE_OK) {
        fprintf(stderr, "[-] FTP upload failed: %s\n", curl_easy_stderror(res));
        return -1;
    }

    printf("[+] Uploaded %s\n", remotePath);

    return 0;
}

#endif
