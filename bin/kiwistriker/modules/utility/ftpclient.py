import ftplib

#!/usr/bin/python3
# Create and return a logged in FTP object
def connectFTP(targetIP, username, password):
    ftp = ftplib.FTP()

    ftp.connect(targetIP, 21)
    ftp.login(username, password)