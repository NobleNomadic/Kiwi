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

    except:
        print(f"[-] Error while connecting to FTP: {e}")


def FTPClientLoop(targetIP, username, password):
    ftp = connectFTP(targetIP, username, password)

    while True:
        command = input(f"FTP@{targetIP}> ")
        if command.lower() == "exit":
            break

    ftp.quit()
    return