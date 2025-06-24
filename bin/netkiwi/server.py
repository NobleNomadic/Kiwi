#!/usr/bin/python3
import socket
import threading
import sys

# Echo server
# Receive data, send it back, and then close connection
# Client handler
def echoHandleClient(clientSocket, clientAddress):
	print(f"[+] Connection from {clientAddress}")

	# Receive data from the client socket
	data = clientSocket.recv(1024)
	print(f"[+] Data received from {clientAddress}: {data.decode()}")

	# Send the same data back to the client socket
	print(f"[*] Sending data to {clientAddress}: {data.decode()}")
	clientSocket.send(data)

	# Close the socket to end connection
	print(f"Closing connection from {clientAddress}")
	clientSocket.close()
	return

# Listen and accept connections
def echoServerListen(listenerPort):
	# Setup a socket to listen for connections
	listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	listenerSocket.bind(("0.0.0.0", listenerPort))
	listenerSocket.listen(5)

	print(f"[*] Echo server listening on port {listenerPort}")

	# Accept connections and get the socket and address
	while True:
		clientSocket, clientAddress = listenerSocket.accept()
		clientThread = threading.Thread(target=echoHandleClient, args=(clientSocket, clientAddress))
		clientThread.start()


# File server can send and receive files
# Upload protcal:
# Client sends upload request
# UPLOAD: <remote filename>
# Server sends response:
# ACCEPT or DENY
# If accepted, client sends data
# 1 line of data for the file (max 2048 bytes)
# Once finished, client sends code to signal end of file
# //:ENDTRANSFER
# All files are stored in a folder called files locally where the script runs
def receiveFileData(clientSocket):
	# Store each line as an item in a list
	fileData = []

	readyMessage = "READY".encode()

	while True:
		clientSocket.send(readyMessage)
		newData = clientSocket.recv(1024).decode()

		if newData == "//:ENDTRANSFER" or len(fileData) == 10000:
			break

		# Add \n to the last position and add newline
		fileData.append(newData + "\n")

	return fileData

def fileHandleClient(clientSocket, clientAddress):
	print(f"[+] Connection from {clientAddress}")

	# Main loop
	while True:
		# Receive request for function
		data = clientSocket.recv(1024).decode()
		print(f"[+] Request sent from {clientAddress}")

		# Convert the message into a set of tokens
		tokenList = data.split(" ")

		if len(tokenList) == 0:
			response = "DENY".encode()
			clientSocket.send(response)

		# FILE UPLOADS
		if len(tokenList) == 2 and tokenList[0] == "UPLOAD":
			# Here you could add logic for scanning the file for malware, add a password, or another system for allowing and denying files
			# For now, it will default to allowing
			data = "ACCEPT".encode()
			clientSocket.send(data)

			remoteFilename = tokenList[1]
			print(f"[*] Receiving file {remoteFilename} from {clientAddress}")

			# Store the contents of the file in a buffer
			fileData = receiveFileData(clientSocket)

			# Write the data to a new file in the files folder
			filePath = f"files/{remoteFilename}"
			with open(filePath, "w") as newFile:
				for line in fileData:
					newFile.write(line)

			# Send final ackknowledgement packet when finished writing data
			finalMessage = "FINISH".encode()
			clientSocket.send(finalMessage)
			s.close()		

def fileServerListen(listenerPort):
	# Setup listener socket
	listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	listenerSocket.bind(("0.0.0.0", listenerPort))
	listenerSocket.listen(5)

	print(f"[*] File server listening on port {listenerPort}")

	# Accept connections
	while True:
		clientSocket, clientAddress = listenerSocket.accept()
		clientThread = threading.Thread(target=fileHandleClient, args=(clientSocket, clientAddress))
		clientThread.start()


def setupServer(listenerPort, serverFlag="echo"):
	if serverFlag == "echo":
		echoServerListen(listenerPort)

	elif serverFlag == "file":
		fileServerListen(listenerPort)

	else:
		print("[-] Invalid server mode")


if __name__ == "__main__":
	if len(sys.argv) != 3:
		print("Usage: python3 server.py <port> <mode>")
		exit(1)

	serverPort = int(sys.argv[1])
	serverFlag = sys.argv[2]
	setupServer(serverPort, serverFlag)
