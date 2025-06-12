#!/usr/bin/python3
import socket
import sys

def findIP(domain):
	try:
		domainIP = socket.gethostbyname(domain)
		print(f"[+] IP of domain: {domainIP}")

		returnData = {
			"ip": domainIP,
			"domain": domain
		}
		return returnData

	except socket.gaierror as e:
		print(f"[-] DNS lookup failed for domain '{domain}': {e}")
		return None

def findDomain(targetIP):
	try:
		domain, _, _ = socket.gethostbyaddr(targetIP)
		print(f"[+] Domain of IP: {domain}")

		returnData = {
			"ip": targetIP,
			"domain": domain
		}
		return returnData

	except socket.herror as e:
		print(f"[-] Reverse DNS lookup failed for IP '{targetIP}': {e}")
		return None

def DNSTool(target, flag):
	if flag.lower() == "domain":
		return findDomain(target)

	elif flag.lower() == "ip":
		return findIP(target)

	else:
		print("[-] Invalid flag")
		return

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print("Usage: python3 dnstool.py <target> <ip|domain>")
		exit(1)

	try:
		DNSTool(sys.argv[1], sys.argv[2])

	except Exception as e:
		print(f"[-] Error: {e}")
