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

# Read local binary data, encode it into chunks, send to server, and decode to rebuild file
def sendFileOverSSH(ssh, localFilePath, remoteFilePath, chunkSize=1024):
	try:
		with open(localFilePath, "rb") as f:
			fileData = f.read()

		b64Data = base64.b64encode(fileData).decode()
		chunks = [b64Data[i:i+chunkSize] for i in range(0, len(b64Data), chunkSize)]

		# Clear remote file
		ssh.exec_command(f"echo -n '' > {remoteFilePath}")

		for idx, chunk in enumerate(chunks):
			# Escape single quotes for shell safety
			safeChunk = chunk.replace("'", "'\"'\"'")
			progress = round((idx + 1) / len(chunks) * 100, 2)
			print(f"[+] Sending chunk {idx + 1}/{len(chunks)} ({progress}%)")

			# Send chunk using printf for better portability
			ssh.exec_command(f"printf '%s' '{safeChunk}' >> {remoteFilePath}")

		print("[*] All chunks sent\n[*] Rebuilding file on remote target")

		# Decode the file remotely
		decodedFilePath = remoteFilePath.replace(".b64", "")
		ssh.exec_command(f"base64 -d {remoteFilePath} > {decodedFilePath}")

		print(f"[+] File rebuilt at {decodedFilePath}")

		# Perform SHA256 integrity check
		localHash = hashlib.sha256(fileData).hexdigest()
		stdin, stdout, stderr = ssh.exec_command(f"sha256sum {decodedFilePath}")
		remoteHash = stdout.read().decode().split()[0]

		if localHash == remoteHash:
			print("[+] File integrity verified successfully")
		else:
			print("[-] File integrity check failed")

	except Exception as e:
		print(f"[-] File transfer failed: {e}")


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
