#!/usr/bin/env python3
import subprocess
import sys

# Add local paths
# If you didn't clone kiwi into a root, change this path
sys.path.append("/kiwi/bin/kiwistriker/modules/exploit")
sys.path.append("/kiwi/bin/kiwistriker/modules/recon")
sys.path.append("/kiwi/bin/kiwistriker/modules/utility")

import bruteclaw, shellshock, xpoverflow
import dirbuster, netmonitor, portscanner, subdomainfind, vulnscan
import dnstool, encrypt, ftpclient, sshclient

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
[dirbuster  ]   [          ]   [dnstool   ]
[subdomain  ]   [          ]   [          ]"""


def processCommand(commandString):
	tokenList = commandString.lower().split(" ")
	bufferData = None	# Buffer used to store the return data of commands

	# Return on empty command
	if len(tokenList) == 0:
		return

	# Admin commands for managing kiwi striker without interacting with modules
	elif tokenList[0] == "exit":
		subprocess.run("clear")
		exit(0)

	elif tokenList[0] == "clear" or tokenList[0] == "reset":
		subprocess.run("clear")
		print(ansiBlue + ansiBold + banner + ansiReset)
		print(commands)

	# Vulnscan
	elif len(tokenList) == 3 and tokenList[0] == "vulnscan":
		if tokenList[1] == "xpoverflow":
			bufferData = vulnscan.checkXPOverflow(tokenList[2])
	
	# Portscanner
	elif len(tokenList) == 3 and tokenList[0] == "portscanner":
		targetIP= tokenList[1]
		portArgument = tokenList[2]
		portList = []

		if portArgument == "common":
			portList = [21, 22, 53, 80, 135, 139, 443, 445, 2222, 8080, 8433]

		elif portArgument == "vuln":
			portList = [21, 22, 23, 445]

		elif portArgument == "router":
			portList = [53, 80, 8080, 443, 8443, 445]

		elif portArgument == "server":
			portList = [3306, 5432, 1521, 1433, 21, 22]

		elif portArgument == "custom":
			try:
				customPorts = input("[>] Enter list of ports: ")
				portList = customPorts.split(" ")

			except:
				print("[-] Enter a valid list")
				return None

		try:
			bufferData = portscanner.scanIP(targetIP, portList)

		except Exception as e:
			print(f"[-] Error: {e}")

	# Directory buster
	elif len(tokenList) == 3 and tokenList[0] == "dirbuster":
		url = tokenList[1]
		wordlistPath = tokenList[2]

		bufferData = dirbuster.findDirectories(url, wordlistPath)

	
	# Subdomain finder
	elif len(tokenList) == 3 and tokenList[0] == "subdomain":
		targetDomain = tokenList[1]
		wordlistPath = tokenList[2]

		bufferData = subdomainfind.findSubdomains(targetDomain, wordlistPath)


	# Network monitor
	elif len(tokenList) == 2 and tokenList[0] == "netmonitor":
		count = tokenList[1]

		try:
			netmonitor.sniffNetwork(int(count))
		except Exception as e:
			print(f"[-] Error: {e}")

	# Network monitor with filters
	elif len(tokenList) >= 3 and tokenList[0] == "netmonitor":
		count = tokenList[1]
		filter = " ".join(tokenList[2:])

		try:
			netmonitor.sniffNetwork(int(count), filter)
		except Exception as e:
			print(f"[-] Error: {e}")

	# DNS utility
	elif len(tokenList) == 3 and tokenList[0] == "dnstool":
		target = tokenList[1]
		flag = tokenList[2]

		bufferData = dnstool.DNSTool(target, flag)

	# Encryption utility
	elif len(tokenList) == 4 and tokenList[0] == "encrypt":
		inFilename = tokenList[1]
		outFilename = tokenList[2]
		key = tokenList[3]

		encrypt.xorFile(inFilename, outFilename, key)

	# FTP client utility
	elif len(tokenList) == 4 and tokenList[0] == "ftp":
		targetIP = tokenList[1]
		username = tokenList[2]
		password = tokenList[3]
		
		ftpclient.FTPClientLoop(targetIP, username, password)

	# SSH client utility
	elif len(tokenList) == 4 and tokenList[0] == "ssh":
		targetIP = tokenList[1]
		username = tokenList[2]
		password = tokenList[3]

		sshclient.SSHClientLoop(targetIP, username, password)


	# Shellshock exploit
	elif len(tokenList) >= 4 and tokenList[0] == "shellshock":
		target = tokenList[1]
		CGIPath = tokenList[2]
		bashCode = tokenList[3:]

		shellshock.shellshockAttack(target, CGIPath, bashCode)

	# Bruteclaw password bruteforcer
	elif len(tokenList) == 5 and tokenList[0] == "bruteclaw":
		targetIP = tokenList[1]
		usernameFilename = tokenList[2]
		passwordFilename = tokenList[3]
		service = tokenList[4]

		bruteclaw.bruteForce(targetIP, usernameFilename, passwordFilename, service)


def mainCLI():
	subprocess.run("clear")
	print(ansiBlue + ansiBold + banner + ansiReset)
	print(commands)

	while True:
		command = input(">>> ")
		processCommand(command)


if __name__ == "__main__":
	mainCLI()