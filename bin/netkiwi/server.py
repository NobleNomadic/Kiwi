#!/usr/bin/python3
import socket
import threading
import sys

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


def setupServer(listenerPort, serverFlag="echo"):
	if serverFlag == "echo":
		echoServerListen(listenerPort)


if __name__ == "__main__":
	if len(sys.argv) != 3:
		print("Usage: python3 server.py <port> <mode>")
		exit(1)

	serverPort = int(sys.argv[1])
	serverFlag = sys.argv[2]
	setupServer(serverPort, serverFlag)
