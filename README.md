# Kiwi
General penetration testing toolkit written in C

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
  - SSH Brute force tool
  - DNS lookup and reverse lookup tool

## Libraries
These are a collection of headers contain function definitions and implementations found in the lib folder.
- configlib: System for getting information from a config file with a header integer at the top, and the rest of the file as a list of integers
- sshlib: Manage SSH sessions
- socketlib: Automate making connections, and sending data over sockets
- dnslib: DNS lookup and reverse lookup
