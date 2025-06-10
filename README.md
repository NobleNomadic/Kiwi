# Kiwi
Kiwi is a cybersecurity toolset containing simple Python tools for penetration testing on Linux.  
**Please do not use these tools for illegal or unethical purposes.**

## Dependencies
To make sure all code runs properly, make sure the following is installed:

- Python 3
- Pip Modules:
  - `scapy`
  - `paramiko`
  - `impacket`
  - `dnspython`

## Using Kiwi
Kiwi is designed to be cloned into a local system and customized.  
You can run some modules and various parts directly, but if you want the simplest experience, run one of the following:

- ```bash
  python3 kiwi/bin/kiwistriker/kiwistriker.py
  ```
  for Kiwi Striker penetration testing multitool

- ```bash
  python3 kiwi/bin/netkiwi/netkiwi.py
  ```
  for Net Kiwi networking tool

## Contents of Kiwi

### Kiwi Striker

Kiwi Striker is a multitool that contains tools for penetration testing within a single place.  
Tools included:

- Recon:
  - Portscanner								– Detects open ports on a target IP
  - Subdomain finder 						– Uses DNS to check if subdomains exist
  - Subdomain bruteforcer 					– Brute forces finding subdomains
  - Network monitor 						– Packet sniffer and analyzer
  - Vulnerability scanner					– Checks if an IP is vulnerable to exploits found in the exploit folder

- Exploit:
  - Bruteforcer for SSH and FTP 			– Returns the credentials of FTP and SSH servers
  - Shellshock implementation 				– An implementation of the Shellshock exploit that attacks Bash applications
  - MS08-067 implementation 				- An implementation of MS08-067 (nicknamed XP-Overflow in Kiwi) for RCE on Windows XP devices

- Utility:
  - File encrypter – Simple encrypter that uses symmetric XOR encryption to directly encrypt the bytes of a file
  - SSH client – Simple SSH client
  - FTP client – Simple FTP client

Each tool is designed to replace an existing one (e.g., portscanner to replace Nmap).  
Kiwi Striker makes it easy to manage all tools and parts of penetration testing in one place.

### Net Kiwi
Net Kiwi is a networking tool with a client and server system.  
The client system allows you to send requests to websites, servers, and make connections.  
The server system allows you to create servers that can listen for reverse shell connections or be configured for automatic responses to certain requests.

### Malware
This folder contains examples of malware and payloads for various purposes.  
Please only run these in isolated virtual machines and do not use them maliciously.

### Share
The `share` folder contains static resources such as wordlists used by other programs.  
You can add your own wordlists or general read-only content here.

### Data
The `data` folder is similar to the `share` folder; however, it is used for non-static files.  
If you want to store custom server rules files, or other data which will change, put it here.

### Etc
The `etc` folder contains configuration files for all parts of Kiwi.

### Log
The `log` folder is used for storing logs and the results of all programs.
Be careful changing anything in this folder, as most programs in Kiwi expect certain files to have a specific structure already defined.
If this repository has added example logs, you should remove the logs, but keep the main array containing all logs.

## Final Note
Kiwi is meant to be customized. I have tried to keep the codebase as simple as possible so that anybody with an understanding of Python can make their own changes directly to the code.  
Also note that Kiwi is designed for Linux only. Some parts may work on Windows, but it is recommended that you use Linux.

If you are interested in contributing to Kiwi, I would recommend you wait until I get it to a minimal working state, which could be a few weeks from when you're reading this.  
By then, a proper structure for adding new modules and programs to Kiwi should be in place.
