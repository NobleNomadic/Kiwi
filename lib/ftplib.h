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
FTPConnection *ftpConnect(const char *targetIP, int port, const char *username, const char *password) {
    // Create the object used for FTP
    FTPConnection *ftpObject = malloc(sizeof(FTPConnection));
    if (!ftpObject) return NULL;

    // Initialize all fields
    ftpObject->curl = NULL;
    ftpObject->urlBase = NULL;
    ftpObject->username = NULL;
    ftpObject->password = NULL;

    // Create the curl object used for the connection
    ftpObject->curl = curl_easy_init();
    if (!ftpObject->curl) {
        fprintf(stderr, "[-] Connection failed: curl_easy_init failed\n");
        free(ftpObject);
        return NULL;
    }

    // Build the URL for the FTP connection
    size_t len = strlen(targetIP) + 20; // ftp://host:port + null + extra space
    ftpObject->urlBase = malloc(len);
    if (!ftpObject->urlBase) {
        fprintf(stderr, "[-] Memory allocation failed\n");
        curl_easy_cleanup(ftpObject->curl);
        free(ftpObject);
        return NULL;
    }
    snprintf(ftpObject->urlBase, len, "ftp://%s:%d/", targetIP, port);

    // Set the username and password
    ftpObject->username = strdup(username);
    ftpObject->password = strdup(password);
    
    if (!ftpObject->username || !ftpObject->password) {
        fprintf(stderr, "[-] Memory allocation failed\n");
        ftpDisconnect(ftpObject);
        return NULL;
    }

    // Set the curl options
    curl_easy_setopt(ftpObject->curl, CURLOPT_USERNAME, ftpObject->username);
    curl_easy_setopt(ftpObject->curl, CURLOPT_PASSWORD, ftpObject->password);

    // Test the connection
    curl_easy_setopt(ftpObject->curl, CURLOPT_URL, ftpObject->urlBase);
    curl_easy_setopt(ftpObject->curl, CURLOPT_NOBODY, 1L); // HEAD request only
    
    CURLcode result = curl_easy_perform(ftpObject->curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "[-] Authentication failed: %s\n", curl_easy_strerror(result));
        ftpDisconnect(ftpObject);
        return NULL;
    }

    // Reset the NOBODY option for future operations
    curl_easy_setopt(ftpObject->curl, CURLOPT_NOBODY, 0L);

    // Return connected object
    return ftpObject;
}

// Upload a file using a connected object
int ftpUploadFile(FTPConnection *ftpObject, const char *localPath, const char *remotePath) {
    // Make sure that FTPConnection exists
    if (!ftpObject || !ftpObject->curl || !localPath || !remotePath) {
        fprintf(stderr, "[-] Invalid parameters\n");
        return -1;
    }

    // Build full URL
    char fullURL[512];
    snprintf(fullURL, sizeof(fullURL), "%s%s", ftpObject->urlBase, remotePath);

    // Open the local file
    FILE *localFile = fopen(localPath, "rb");
    if (!localFile) {
        fprintf(stderr, "[-] fopen: Could not open local file %s\n", localPath);
        return -1;
    }

    // Build the request
    curl_easy_setopt(ftpObject->curl, CURLOPT_URL, fullURL);
    curl_easy_setopt(ftpObject->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(ftpObject->curl, CURLOPT_READDATA, localFile);

    // Make the request
    CURLcode result = curl_easy_perform(ftpObject->curl);
    fclose(localFile);

    if (result != CURLE_OK) {
        fprintf(stderr, "[-] FTP upload failed: %s\n", curl_easy_strerror(result));
        return -1;
    }

    printf("[+] Upload complete: %s\n", remotePath);
    return 0;
}

// Download a file using a connected object
int ftpDownloadFile(FTPConnection *ftpObject, const char *remotePath, const char *localPath) {
    if (!ftpObject || !ftpObject->curl || !remotePath || !localPath) {
        fprintf(stderr, "[-] Invalid parameters\n");
        return -1;
    }

    // Build full URL
    char fullURL[512];
    snprintf(fullURL, sizeof(fullURL), "%s%s", ftpObject->urlBase, remotePath);

    // Open local file for writing
    FILE *localFile = fopen(localPath, "wb");
    if (!localFile) {
        fprintf(stderr, "[-] fopen: Could not create local file %s\n", localPath);
        return -1;
    }

    // Setup curl for download
    curl_easy_setopt(ftpObject->curl, CURLOPT_URL, fullURL);
    curl_easy_setopt(ftpObject->curl, CURLOPT_UPLOAD, 0L);
    curl_easy_setopt(ftpObject->curl, CURLOPT_WRITEDATA, localFile);

    // Perform the download
    CURLcode result = curl_easy_perform(ftpObject->curl);
    fclose(localFile);

    if (result != CURLE_OK) {
        fprintf(stderr, "[-] FTP download failed: %s\n", curl_easy_strerror(result));
        return -1;
    }

    printf("[+] Download complete: %s\n", localPath);
    return 0;
}

// Run an FTP command (like LIST, PWD, etc.)
int ftpRunCommand(FTPConnection *ftpObject, const char *command) {
    // Make sure that FTPConnection exists
    if (!ftpObject || !ftpObject->curl || !command) {
        fprintf(stderr, "[-] Invalid parameters\n");
        return -1;
    }

    // Build URL with command
    char fullURL[512];
    snprintf(fullURL, sizeof(fullURL), "%s", ftpObject->urlBase);

    // Setup curl for the command
    curl_easy_setopt(ftpObject->curl, CURLOPT_URL, fullURL);
    curl_easy_setopt(ftpObject->curl, CURLOPT_UPLOAD, 0L);
    curl_easy_setopt(ftpObject->curl, CURLOPT_WRITEDATA, stdout);

    // Add custom FTP command
    struct curl_slist *commands = NULL;
    commands = curl_slist_append(commands, command);
    curl_easy_setopt(ftpObject->curl, CURLOPT_QUOTE, commands);

    // Execute command
    CURLcode result = curl_easy_perform(ftpObject->curl);
    
    // Clean up
    curl_slist_free_all(commands);

    if (result != CURLE_OK) {
        fprintf(stderr, "[-] Failed to execute command: %s\n", curl_easy_strerror(result));
        return -1;
    }

    return 0;
}

// Clean up and disconnect
void ftpDisconnect(FTPConnection *ftpObject) {
    if (!ftpObject) return;
    
    if (ftpObject->curl) {
        curl_easy_cleanup(ftpObject->curl);
    }
    if (ftpObject->urlBase) {
        free(ftpObject->urlBase);
    }
    if (ftpObject->username) {
        free(ftpObject->username);
    }
    if (ftpObject->password) {
        free(ftpObject->password);
    }
    free(ftpObject);
}

#endif
