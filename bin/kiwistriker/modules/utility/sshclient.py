#!/usr/bin/python3
import paramiko

# Connect to SSH server and return connected object
def connectSSH(targetIP, username, password):
	try:
		ssh = paramiko.SSHClient()
		ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
		ssh.connect(hostname=targetIP, port=22, username=username, password=password, timeout=5)

		return ssh

	except Exception as e:
		print(f"[-] Error: {e}")

def sshClientLoop(targetIP, username, password)