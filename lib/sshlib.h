#ifndef sshlib_h
#define sshlib_h

#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
    ssh_session session;
    ssh_channel channel;
} SSHConnection;

// Make a connection and return an SSH object
SSHConnection *sshConnect(const char *targetIP, int port, const char *username, const char *password) {
    // Create the main object to connect
    SSHConnection *sshObject = malloc(sizeof(SSHConnection));
    if (sshObject == NULL) return NULL;

    // Setup the object as a new SSH object
    sshObject->session = ssh_new();
    sshObject->channel = NULL;
    if (sshObject->session == NULL) {
        free(sshObject);
        return NULL;
    }

    // Setup the options for making a connection
    ssh_options_set(sshObject->session, SSH_OPTIONS_HOST, targetIP); // Set the target host
    ssh_options_set(sshObject->session, SSH_OPTIONS_PORT, &port);    // Set the port
    ssh_options_set(sshObject->session, SSH_OPTIONS_USER, username); // Set the username

    // Make the connection to the target
    if (ssh_connect(sshObject->session) != SSH_OK) {
        fprintf(stderr, "[-] Connection failed: %s\n", ssh_get_error(sshObject->session));
        ssh_free(sshObject->session);
        free(sshObject);
        return NULL;
    }

    // Authenticate the ssh object
    if (ssh_userauth_password(sshObject->session, NULL, password) != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "[-] Authentication failed: %s\n", ssh_get_error(sshObject->session));
        ssh_free(sshObject->session);
        free(sshObject);
        return NULL;
    }

    // Return connected object
    return sshObject;
}

// Run a command on an SSH session
int sshRunCommand(SSHConnection *sshObject, const char *command) {
    // Make sure that SSHConnection exists
    if (!sshObject || !sshObject->session || !command) {
        fprintf(stderr, "[-] Invalid parameters\n");
        return -1;
    }

    // Create new channel
    ssh_channel channel = ssh_channel_new(sshObject->session);
    if (!channel) {
        fprintf(stderr, "[-] Could not create channel\n");
        return -1;
    }

    // Open new channel
    if (ssh_channel_open_session(channel) != SSH_OK) {
        fprintf(stderr, "[-] Unable to open channel: %s\n", ssh_get_error(sshObject->session));
        ssh_channel_free(channel);
        return -1;
    }

    // Execute command
    if (ssh_channel_request_exec(channel, command) != SSH_OK) {
        fprintf(stderr, "[-] Failed to execute command: %s\n", ssh_get_error(sshObject->session));
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return -1;
    }

    // Read the output
    char buffer[256];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, nbytes, stdout);
    }

    // Clean up
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return 0;
}

// Upload a file over SSH using SFTP
int sshUploadFile(SSHConnection *sshObject, const char *localPath, const char *remotePath) {
    if (!sshObject || !sshObject->session || !localPath || !remotePath) {
        fprintf(stderr, "[-] Invalid parameters\n");
        return -1;
    }

    // Create SFTP session
    sftp_session sftp = sftp_new(sshObject->session);
    if (!sftp) {
        fprintf(stderr, "[-] sftp_new: %s\n", ssh_get_error(sshObject->session));
        return -1;
    }

    if (sftp_init(sftp) != SSH_OK) {
        fprintf(stderr, "[-] sftp_init: %s\n", ssh_get_error(sshObject->session));
        sftp_free(sftp);
        return -1;
    }

    // Open local file
    FILE *fp = fopen(localPath, "rb");
    if (!fp) {
        perror("[-] fopen");
        sftp_free(sftp);
        return -1;
    }

    // Create remote file
    sftp_file file = sftp_open(sftp, remotePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (!file) {
        fprintf(stderr, "[-] sftp_open: %s\n", ssh_get_error(sshObject->session));
        fclose(fp);
        sftp_free(sftp);
        return -1;
    }

    // Transfer data
    char buffer[4096];
    size_t nread;
    while ((nread = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        if (sftp_write(file, buffer, nread) != nread) {
            fprintf(stderr, "[-] sftp_write error: %s\n", ssh_get_error(sshObject->session));
            sftp_close(file);
            fclose(fp);
            sftp_free(sftp);
            return -1;
        }
    }

    sftp_close(file);
    fclose(fp);
    sftp_free(sftp);

    printf("[+] Upload complete: %s\n", remotePath);
    return 0;
}

// Download a file with SFTP
int sshDownloadFile(SSHConnection *sshObject, const char *remotePath, const char *localPath) {
    if (!sshObject || !sshObject->session || !remotePath || !localPath) {
        fprintf(stderr, "[-] Invalid parameters\n");
        return -1;
    }

    sftp_session sftp = sftp_new(sshObject->session);
    if (!sftp) {
        fprintf(stderr, "[-] sftp_new: %s\n", ssh_get_error(sshObject->session));
        return -1;
    }

    if (sftp_init(sftp) != SSH_OK) {
        fprintf(stderr, "[-] sftp_init: %s\n", ssh_get_error(sshObject->session));
        sftp_free(sftp);
        return -1;
    }

    // Open remote file
    sftp_file file = sftp_open(sftp, remotePath, O_RDONLY, 0);
    if (!file) {
        fprintf(stderr, "[-] sftp_open: %s\n", ssh_get_error(sshObject->session));
        sftp_free(sftp);
        return -1;
    }

    FILE *fp = fopen(localPath, "wb");
    if (!fp) {
        perror("[-] fopen");
        sftp_close(file);
        sftp_free(sftp);
        return -1;
    }

    // Read and write
    char buffer[4096];
    int nread;
    while ((nread = sftp_read(file, buffer, sizeof(buffer))) > 0) {
        if (fwrite(buffer, 1, nread, fp) != (size_t)nread) {
            fprintf(stderr, "[-] fwrite failed\n");
            fclose(fp);
            sftp_close(file);
            sftp_free(sftp);
            return -1;
        }
    }

    if (nread < 0) {
        fprintf(stderr, "[-] sftp_read error: %s\n", ssh_get_error(sshObject->session));
        fclose(fp);
        sftp_close(file);
        sftp_free(sftp);
        return -1;
    }

    fclose(fp);
    sftp_close(file);
    sftp_free(sftp);

    printf("[+] Download complete: %s\n", localPath);
    return 0;
}

// Disconnect from SSH and free memory
void sshDisconnect(SSHConnection *conn) {
    if (!conn) return;
    if (conn->channel) {
        ssh_channel_close(conn->channel);
        ssh_channel_free(conn->channel);
    }
    ssh_disconnect(conn->session);
    ssh_free(conn->session);
    free(conn);
}

#endif
