import socket
import random
import sys

def checkPort(targetIP, port):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.settimeout(2)

	result = s.connect_ex((targetIP, port))
	s.close()

	if result == 0:
		print(f"[+] Port {port} is open")
		return True		
	return False

def scanIP(targetIP, portList):
	print(f"[*] Scanning {len(portList)} ports on {targetIP}")

	targetOpenPorts= []
	random.shuffle(portList)

	for port in portList:
		portOpen = checkPort(targetIP, port)

		if portOpen:
			targetOpenPorts.append(port)

	return targetOpenPorts

if __name__ == "__main__":
	scanIP("127.0.0.1", list(range(65535)))