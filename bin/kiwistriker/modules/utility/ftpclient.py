import ftplib

#!/usr/bin/python3
# Create and return a logged in FTP object
def connectFTP(targetIP, username, password):
    try:
        ftp = ftplib.FTP()
        ftp.connect(targetIP, 21)
        ftp.login(username, password)

        print("[+] Logged into FTP")
        return ftp

    except Exception as e:  # You missed catching the exception correctly
        print(f"[-] Error while connecting to FTP: {e}")
        return None  # Always return something

def FTPList(ftp):
    try:
        ftp.retrlines('LIST')
    except Exception as e:
        print(f"[-] Error listing directory: {e}")


def FTPDownload(ftp, remoteFile, localFile):
    try:
        with open(localFile, 'wb') as file:
            ftp.retrbinary(f'RETR {remoteFile}', file.write)
        print(f"[+] Downloaded {remoteFile} to {localFile}")
    except Exception as e:
        print(f"[-] Error downloading file: {e}")


def FTPupload(ftp, localFile, remoteFile):
    try:
        with open(localFile, 'rb') as file:
            ftp.storbinary(f'STOR {remoteFile}', file)
        print(f"[+] Uploaded {localFile} to {remoteFile}")
    except Exception as e:
        print(f"[-] Error uploading file: {e}")


def FTPClientLoop(targetIP, username, password):
    ftp = connectFTP(targetIP, username, password)
    
    if ftp == None:
        return

    while True:
        command = input(f"FTP@{targetIP}> ")
        if command.lower() == "exit":
            break

        tokenList = command.lower().split()
        if len(tokenList) == 0:
            continue

        elif len(tokenList) == 3 and tokenList[0] == "upload":
            FTPupload(ftp, tokenList[1], tokenList[2])

        elif len(tokenList) == 3 and tokenList[0] == "download":
            FTPDownload(ftp, tokenList[1], tokenList[2])

        elif tokenList[0] == "list":
            FTPList(ftp)

    print("[*] Exiting FTP")
    ftp.quit()
    return