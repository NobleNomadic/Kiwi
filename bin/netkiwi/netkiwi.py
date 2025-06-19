#!/usr/bin/python3
<<<<<<< HEAD
import subprocess
import client, server

# Color definitions
ansiGreen = "\x1b[32m"
ansiBlue = "\x1b[34m"
ansiRed = "\x1b[31m"
ansiYellow = "\x1b[33m"
ansiBold = "\x1b[1m"
ansiReset = "\x1b[0m"

banner = """TEMP"""
commands = """COMMAND LIST"""


def processCommand(command):
    tokenList = command.split(" ")

    if len(tokenList) == 0:
        return

    # Client utility
    if len(tokenList) == 4 and tokenList[0] == "client":
        try:
            targetIP = tokenList[1]
            port = int(tokenList[2])
            mode = tokenList[3]
            
            client.netkiwiClient(targetIP, port, mode)

        except:
            pass


def mainCLI():
    subprocess.run("cls")
    print(ansiGreen + ansiBold + banner + ansiReset)
    print(commands)

    while True:
        command = input(">>> ")
        processCommand(command)


if __name__ == "__main__":
    mainCLI()
=======
import client, server
>>>>>>> 7d8a803 (Working on server system)
