import socket
import threading
import os
import sys

def echoHandleClient(clientSocket, clientAddress):
	print(f"[+] Connection from {clientAddress}")
	data = clientSocket.recv(1024)
	print(f"[+] Data received from {clientAddress}: {data.decode()}")
	print(f"[*] Sending data to {clientAddress}: {data.decode()}")
	clientSocket.send(data)
	print(f"[+] Closing connection from {clientAddress}")
	clientSocket.close()

def echoServerListen(listenerPort):
	listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	listenerSocket.bind(("0.0.0.0", listenerPort))
	listenerSocket.listen(5)
	print(f"[*] Echo server listening on port {listenerPort}")

	while True:
		clientSocket, clientAddress = listenerSocket.accept()
		threading.Thread(target=echoHandleClient, args=(clientSocket, clientAddress)).start()

def receiveFileData(clientSocket):
	fileData = []
	while True:
		clientSocket.send(b"READY")
		newData = clientSocket.recv(1024).decode()
		if newData == "//:ENDTRANSFER" or len(fileData) >= 10000:
			break
		fileData.append(newData + "\n")
	return fileData

def fileHandleClient(clientSocket, clientAddress):
	print(f"[+] Connection from {clientAddress}")
	os.makedirs("files", exist_ok=True)

	try:
		data = clientSocket.recv(1024).decode()
		print(f"[+] Request sent from {clientAddress}")
		tokenList = data.split(" ")

		if len(tokenList) == 2 and tokenList[0] == "UPLOAD":
			clientSocket.send(b"ACCEPT")
			remoteFilename = tokenList[1]
			print(f"[*] Receiving file {remoteFilename} from {clientAddress}")

			fileData = receiveFileData(clientSocket)
			filePath = f"files/{remoteFilename}"
			with open(filePath, "w") as f:
				f.writelines(fileData)

			clientSocket.send(b"FINISH")
		else:
			clientSocket.send(b"DENY")
	except Exception as e:
		print(f"[-] Error: {e}")
	finally:
		clientSocket.close()

def fileServerListen(listenerPort):
	listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	listenerSocket.bind(("0.0.0.0", listenerPort))
	listenerSocket.listen(5)
	print(f"[*] File server listening on port {listenerPort}")

	while True:
		clientSocket, clientAddress = listenerSocket.accept()
		threading.Thread(target=fileHandleClient, args=(clientSocket, clientAddress)).start()

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
		sys.exit(1)
	port = int(sys.argv[1])
	mode = sys.argv[2].lower()
	setupServer(port, mode)