# Kiwi
Kiwi is a cybersecurity toolset containing tools of all kind for penetration testing on Linux.
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
Kiwi is designed to be cloned into the root directory system and customized.
It is important that this done to make sure that the local imports work. Currently, kiwi striker will import from /kiwi/bin/kiwistriker/modules, and cloning the repo into another directory won't work unless you modify the import paths in kiwi striker  
You can run some modules and various parts directly, but if you want the simplest experience, run one of the following:

- ```bash
  python3 kiwi/bin/kiwistriker/kiwistriker.py
  ```
  for Kiwi Striker penetration testing multitool, or use

- ```bash
  python3 kiwi/bin/netkiwi/netkiwi.py
  ```
  for Net Kiwi networking tool

## Contents of Kiwi

### Bin Folder
This is the folder where all tools are stored along with their source code. You can add your own tool in a new folder inside bin.

### Kiwi Striker

Kiwi Striker is a multitool that contains tools for penetration testing within a single place.  
Tools included:

- Recon:
  - Portscanner								    – Detects open ports on a target IP
  - Subdomain finder 						  – Uses DNS to check if subdomains exist
  - Subdomain bruteforcer 				– Brute forces finding subdomains
  - Network monitor 						  – Packet sniffer and analyzer

- Exploit:
  - Bruteforcer for SSH and FTP 			– Returns the credentials of FTP and SSH servers
  - Packer                            - Allows you to write scripts to automate connections, and send and receive data

- Utility:
  - File encrypter – Simple encrypter that uses symmetric XOR encryption to directly encrypt the bytes of a file
  - SSH client – Simple SSH client
  - FTP client – Simple FTP client
  - DNS tool   - Tool for converting IPs to domains, and domains to IPs

Each tool is designed to replace an existing one (e.g., portscanner to replace Nmap).  
Kiwi Striker makes it easy to manage all tools and parts of penetration testing in one place.

### Net Kiwi
Net Kiwi is a networking tool with a client and server system.  
The client system allows you to send requests to websites, servers, and make connections.  
The server system allows you to create servers that can listen for reverse shell connections or be configured for automatic responses to certain requests.

### Share
The `share` folder contains static resources such as wordlists used by other programs.  
You can add your own wordlists or general read-only content here.

### Data
The `data` folder is similar to the `share` folder; however, it is used for non-static files.  
If you want to store custom server rules files, packer scripts, or data that will change, put it here.

### Etc
The `etc` folder contains configuration files for all parts of Kiwi.

### Log
The `log` folder is used for storing logs and the results of all programs.
Be careful changing anything in this folder, as most programs in Kiwi expect certain files to have a specific structure already defined.
If this repository has added example logs, you should remove the logs, but keep the main array containing all logs.

## Final Notes
Kiwi is meant to be customized. I have tried to keep the codebase as simple as possible so that anybody with an understanding of Python can make their own changes directly to the code.
Everybody, especially beginners are welcome to add their tools to the kiwi repository. If you are learning how to create your own cybersecurity tools, feel free to put a copy of the code in the bin folder. By doing this, you can get quicker feedback from other people who work on kiwi, and get your code into a more public space.
Also note that Kiwi is designed for Linux only. You may be able to get some parts to work on Windows, but I have not created anything with Windows in mind.
