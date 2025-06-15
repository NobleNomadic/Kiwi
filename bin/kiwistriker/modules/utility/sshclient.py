#!/usr/bin/python3
import paramiko
import base64
import os
import hashlib

# Connect to SSH server and return connected object
def connectSSH(targetIP, username, password):
	try:
		ssh = paramiko.SSHClient()
		ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
		ssh.connect(hostname=targetIP, port=22, username=username, password=password, timeout=5)

		return ssh

	except Exception as e:
		print(f"[-] Connection Error: {e}")
		return None

# Run a standard shell command
def runSSHCommand(ssh, command):
	print(f"[*] Executing '{command}'")

	try:
		stdin, stdout, stderr = ssh.exec_command(command)

		# Wait for command to complete
		exitStatus = stdout.channel.recv_exit_status()

		output = stdout.read().decode()
		error = stderr.read().decode()

		if output.strip():
			print(output.strip())
		if error.strip():
			print(f"[!] Error:\n{error.strip()}")

	except Exception as e:
		print(f"[-] Command execution failed: {e}")

def sendFileOverSSH(ssh, localFilePath, remoteFilePath):
	try:
		sftp = ssh.open_sftp()

		print(f"[*] Uploading '{localFilePath}' to '{remoteFilePath}'")
		sftp.put(localFilePath, remoteFilePath)

		print("[+] File uploaded successfully.")
		sftp.close()

	except FileNotFoundError:
		print(f"[-] Local file '{localFilePath}' not found.")
	except Exception as e:
		print(f"[-] Failed to send file: {e}")
		traceback.print_exc()


def SSHClientLoop(targetIP, username, password):
	ssh = connectSSH(targetIP, username, password)

	if ssh is None:
		return

	while True:
		command = input(f"SSH@{targetIP}> ")
		if command.strip().lower() == "exit":
			break

		tokenList = command.strip().split(" ")

		if len(tokenList) == 0:
			continue

		cmd = tokenList[0].lower()

		if cmd == "run" and len(tokenList) >= 2:
			commandToExecute = " ".join(tokenList[1:])
			runSSHCommand(ssh, commandToExecute)

		elif cmd == "send" and len(tokenList) == 3:
			localFilePath = tokenList[1]
			remoteFilePath = tokenList[2]
			sendFileOverSSH(ssh, localFilePath, remoteFilePath)

	print("[*] Exiting SSH")
	ssh.close()
	return
