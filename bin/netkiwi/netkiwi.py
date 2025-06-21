#!/usr/bin/python3
import subprocess
import sys
import client, server

# Color definitions
ansiGreen = "\x1b[32m"
ansiBlue = "\x1b[34m"
ansiRed = "\x1b[31m"
ansiYellow = "\x1b[33m"
ansiReset = "\x1b[0m"

banner = """
    _   ______________   __ __ _____       ______
   / | / / ____/_  __/  / //_//  _/ |     / /  _/
  /  |/ / __/   / /    / ,<   / / | | /| / // /  
 / /|  / /___  / /    / /| |_/ /  | |/ |/ // /   
/_/ |_/_____/ /_/    /_/ |_/___/  |__/|__/___/"""
commands = """
[client <IP> <port> <basic|http>              ]
[server <port> <mode>                         ]"""

def processCommand(command):
    tokenList = command.split(" ")

    if len(tokenList) == 0:
        return

    # Admin commands for controlling netkiwi
    if tokenList[0] == "exit":
        subprocess.run("clear")
        exit(0)

    elif tokenList[0] == "clear" or tokenList[0] == "reset":
        subprocess.run("clear")
        print(ansiGreen + banner + ansiReset)
        print(commands)

    # Client command
    elif len(tokenList) == 4 and tokenList[0] == "client":
        try:
            targetIP = tokenList[1]
            port = int(tokenList[2])
            mode = tokenList[3]
            
            client.netkiwiClient(targetIP, port, mode)

        except:
            pass

    # Server command
    elif len(tokenList) == 3 and tokenList[0] == "server":
        try:
            port = int(tokenList[1])
            mode = tokenList[2]

            server.setupServer(port, mode)

        except:
            pass

def mainCLI(silentMode=False):
    subprocess.run("clear")

    if silentMode == False:
        print(ansiGreen + banner + ansiReset)
        print(commands)

    while True:
        command = input(">>> ")
        processCommand(command)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        mainCLI(False)

    elif len(sys.argv) == 2 and sys.argv[1] == "silent":
        mainCLI(True)
