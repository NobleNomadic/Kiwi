# Kiwi
General penetration testing toolkit written in C.
Contains tools for recon, exploit, payloads, and utilities for general network pentesting.
Please do not use anything here malicously.

## Installing Kiwi
Install kiwi by cloning this repository. Each tool is inside its own folder, which will be found the recon, exploit, payload or utility folder.
Each tools folder will have the C source code, and a compiled binary file for that tool.
The lib folder will contain all the wrappers needed for automating things like TCP connections, and DNS lookups.

## Tools
- Recon
  - Portscanner
  - DNS subdomain finder
  - Hidden webpage and directory finder
- Exploit
  - Webdrop exploit
  - Packer scripting tool for automating connecting and sending packets
- Payload
  - SQLi toolkit
  - Reverse shell toolkit
- Utility
  - SSH Client
  - General Brute force tool
  - DNS lookup and reverse lookup tool
  - NetCatcher packet analysis tool

## Libraries
These are a collection of headers contain function definitions and implementations found in the lib folder.
- configlib: System for getting information from a config file with a header integer at the top, and the rest of the file as a list of integers
- sshlib: Manage SSH client session and actions
- ftplib: Manage FTP client connections
- socketlib: Automate making connections, and sending data over sockets
- dnslib: DNS lookup and reverse lookup

## Using Kiwi
### Portscanner
Usage: ./portscanner [IP] [Scan File]
IP: The target IP address that you want to scan
Scan File: The file containing the data to be scanned.
           This file contains a header at the top followed by a list of ports to scan
           The header is the sum of all flags that you want to use in the scan.
           These flags enable features of the portscanner.<br>
Flags:
- 1: Enable logging
- 2: Verbose text output
- 4: Verbose logging
- 8: Longtimeout
For example, if you want to print all closed ports, and log only the open ones, you would use the header 3.
This flag is the sum of the number for verbose text output (2) + the flag for regular logging (1). <br>

Example:
```bash
./portscanner 127.0.0.1 basic
```
"basic" scanfile:
```scanfile
1
21
22
53
```
This will scan port 21, 22, and 53 with standard logging.

### DNSFind
Usage: ./dnsfind [Domain] [Wordlist]
Domain: The domain which you want to find subdomains of
Wordlist: File containing list of domains to attempt to find<br>

This tool will attempt to lookup subdomains using DNS.
This tool can't find hidden directories or domains that are not found part of regular DNS.<br>

Example:
```bash
./dnsfind website.com domains.txt
```
This could find a domain like shop.website.com, if shop was in the wordlist.

### Webfind
Usage: ./webfind [Target URL] [Wordlist]
Target URL: The URL you want to find hidden pages on
Wordlist: The list of directories/items to try and find<br>

This tool will send HTTP requests to the target URL + each word from the wordlist.
It will return the code received from the server after the request is made.<br>

This is best used when you want to find specific pages, hidden directories, and specific pages.
You will likely need to create specific wordlists for each website.
It works well with finding pages on php websites that might include ? symbols.

Example:
```bash
./webfind website.com sites.txt
```
Sites.txt:
```txt
?users=admin
js/
admin/
images/
```
This could find the URL for website.com/images/

### Packer
Usage: ./packer [Packer script]
Packer script: A script which is used to automate socket connection.

Packer syntax:
```packer
connect [IP]
send [Data]
recv
close
```
The scripts can be used to send HTTP requests, or communicate over simple protocals.
You can also use it to trigger a simple buffer overflows, or use it for fuzzing.
Note that hostnames are not supported. You need to use IP addresses.
Use DNSTool if needed to find the IP of a domain.

### Webdrop
Webdrop is an exploit which involves attacking a server using both ftp and web servers.
Read about it [here.](exploit/webdrop/webdrop.md)
Use the [python script](exploit/webdrop/webdrop.py) in the same folder to automate it.<br>
Step 1:
Configure the [bash CGI script payload](payload/rshell/cgibash.sh) with the correct IP and port.<br>
Step 2:
Change any needed settings to do with the path and request system in the python script.<br>
Step 3:
Run the python script<br>

From testing I have found that Webdrop usually doesn't work due to needing high permissions.
The server owner will need to accidently configure the FTP server to be allowed to write to the web server folders.

### SQLi
Contains a basic set of SQLi commands that might come in userful.
Refer to these when stuck on SQLi parts of CTFs, but this list is not very extensive.
I would recommend the SQLi cheat sheet in swisskyrepo's PayloadsAllTheThings repository.

### Rshell
This folder has some basic reverse shell payloads.
You can try uploading them using webdrop, or just use them as a reference.
I would recommend using an online reverse shell generator for specific situations.

### Bruteclaw
Multithreaded SSH and FTP brute forcing tool.
Usage: ./bruteclaw [IP] [Username file] [Password file] [Service]<br>
IP: The IP you want to attack. This works better with an IP than host. Use DNSTool if needed
Username file: List of usernames to use
Password file: List of passwords to try
Service: Either `FTP` or `SSH` depending on what your attacking.

I would recommend adjusting the max threads variable and recompiling the tool based on your CPU.

### DNSTool
Simple tool to resolve and reverse resolve domains and IPs.
Usage: ./dnstool [Information] [Action]
Information: Either an IP address or domain
Action: `resolve` or `reverse` depending on the data you want to find with the input information.
Use this when writing packer scripts or researching.

### SSHClient
Usage: ./sshclient [IP] [Username] [Password]
Note that running this will save the password you entered to bash_history.
Minimal SSH client program. Only use this to simplify uploading or downloading files, or running one off commands.
The default Linux SSH program works better, this one is quite glitchy and unstable.

### NetCatcher
Usage: ./netcatcher [Packet count]
Packet count: Number of packets to try and sniff
A packet sniffing tool able to capture information about IP headers, similar to wireshark.
It can capture source and destination IPs, and the information within a packet.
This means it can be used to capture sensative information sent over HTTP such as passwords.
