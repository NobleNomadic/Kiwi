# Kiwi
Kiwi is a cybersecurity toolset containing simple python tools for penetration testing for linux.
Please do not use these tools for ilegal or unethical purposes.

## Using kiwi
Kiwi is designed to be cloned into a local system and customised.
You can run some modules and various parts directly, but if you want the simplest experience, run one of the following:
- ```python3 kiwi/bin/kiwistriker/kiwistriker.py``` for Kiwi Striker penetration testing multitool
- ```python3 kiwi/bin/netkiwi/netkiwi.py``` for Net Kiwi networking tool

## Contents of kiwi
### Kiwi Striker
Kiwi Striker is a multitool that contains tools for penetration testing within a single place.
Tools included:
- Recon:
	- Portscanner
	- Subdomain finder
	- Subdomain bruteforcer
	- Network monitor
	- Vulnerability scanner
- Exploit:
	- Bruteforcer for SSH and FTP
	- Shellshock implementation
	- MS08-067 implementation (XP-Overflow)
- Utility
	- File encrypter
	- SSH client
	- FTP client

Each tool is designed to replace an exisiting one. (E.G. portscanner to replace nmap)
Kiwi Striker makes it easy to manage all tools and parts of penetration testing in one place.

### Net Kiwi
Net Kiwi is a networking tool with a client and server system.
The client system allows you to send requests to websites, servers, and make connections.
The server system allows you to create servers that can listen for reverse shell connections, or configured for automatic responses to certain requests.

### Malware
This folder contains examples of malware and payloads for various purposes.
Please only run these in isolated virtual machines and do not use them malicously.

### Share
The share folder contains static resources such as wordlists used by other programs.
You can add your own wordlists or general read-only content here.

### Data
The data folder is similar to the share folder, however is used for non-static files.
If you want store custom server rules files, or other data which will change, put it here.

### Etc
The etc folder contains configuration files for all parts of kiwi.

### Log
The log folder is used for storing logs and the result of all programs
Be careful changing anything in this folder as most programs in kiwi expect certain files to have a certain structure already defined.

## Final Note
Kiwi is meant to be customised. I have tried to keep the codebase as simple as possible so that anybody with an understanding of python can make their own changes directly to the code.
Also note that kiwi is designed for Linux only. Some parts may work on Windows, but it is recomended that you use Linux.
If you are interested in contributing to Kiwi, I would recommend you wait until I get it to a minimal working state which could be a few weeks from when your reading this.
By then, a proper structure for adding new modules and programs to Kiwi should be in place.