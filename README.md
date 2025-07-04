# Kiwi
General penetration testing toolkit written in C.

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
  - WebDrop exploit
  - Packer scripting tool for automating connecting and sending packets
- Payload
  - SQLi toolkit
  - Reverse shell toolkit
- Utility
  - SSH Client
  - FTP Client
  - General Brute force tool
  - DNS lookup and reverse lookup tool

## Libraries
These are a collection of headers contain function definitions and implementations found in the lib folder.
- configlib: System for getting information from a config file with a header integer at the top, and the rest of the file as a list of integers
- sshlib: Manage SSH client session
- ftplib: Manage FTP client session
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
js/
admin/
images/
```
This could find the URL for website.com/images/
