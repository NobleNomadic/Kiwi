#!usr/bin/python3
import subprocess

ansiGreen = "\x1b[32m"
ansiBlue = "\x1b[34m"
ansiRed = "\x1b[31m"
ansiYellow = "\x1b[33m"
ansiBold = "\x1b[1m"
ansiReset = "\x1b[0m"

banner = """
    __ __ _____       ______
   / //_//  _/ |     / /  _/
  / ,<   / / | | /| / // /
 / /| |_/ /  | |/ |/ // / 
/_/_|_/___/__|__/|__/___/_ __ __________
  / ___/_  __/ __ \\/  _/ //_// ____/ __ \\
  \\__ \\ / / / /_/ // // ,<  / __/ / /_/ /
 ___/ // / / _, _// // /| |/ /___/ _, _/
/____//_/ /_/ |_/___/_/ |_/_____/_/ |_|"""

commands = """
RECON            EXPLOIT       UTILITY
[portscanner]   [xpoverflow]   [encrypt   ]
[vulnscan   ]   [bruteclaw ]   [sshclient ]
[netmonitor ]   [shellshock]   [ftpclient ]
[dirbuster  ]   [          ]   [          ]
[subdomains ]   [          ]   [          ]"""


def processCommand(commandString):
	tokenList = commandString.lower().split(" ")

	if len(tokenList) == 0:
		return

	elif tokenList[1] == "exit":
		subprocess.run("clear")
		exit(0)


def mainCLI():
	subprocess.run("clear")
	print(ansiBlue + ansiBold + banner + ansiReset)
	print(commands)

	while True:
		command = input(">>> ")
		processCommand(command)


if __name__ == "__main__":
	mainCLI()