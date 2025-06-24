#!/usr/bin/python3
import socket
import sys

def connectHTTP(targetIP, port):
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.settimeout(3)

    try:
        clientSocket.connect((targetIP, port))
        print(f"[+] Connected to {targetIP}:{port}")
    except Exception as e:
        print(f"[-] Connection error: {e}")
        return

    hostHeader = f"Host: {targetIP}"
    userAgent = "User-Agent: SimplePythonClient/1.0"
    connectionHeader = "Connection: close"

    # Form full HTTP GET request
    httpRequest = f"GET / HTTP/1.1\r\n{hostHeader}\r\n{userAgent}\r\n{connectionHeader}\r\n\r\n"
    clientSocket.send(httpRequest.encode())

    # Receive response in chunks
    response = b""
    while True:
        try:
            chunk = clientSocket.recv(4096)
            if not chunk:
                break
            response += chunk
        except socket.timeout:
            clientSocket.close()
            break

    print(response.decode(errors="replace"))
    clientSocket.close()
    return

def basicClientConnectionLoop(clientSocket, targetIP):
    while True:
        try:
            command = input(f"{targetIP}> ")
            if command.lower() in ('exit', 'quit'):
                break

            clientSocket.send(command.encode())

            # Receive response after sending
            response = clientSocket.recv(4096)
            if not response:
                print("[*] Connection closed by remote host")
                break
            print(response.decode(errors="replace"))

        except KeyboardInterrupt:
            print("\n[*] User terminated connection.")
            break

        except Exception as e:
            print(f"[-] Error: {e}")
            break
    return

def connectBasic(targetIP, port):
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.settimeout(3)

    try:
        clientSocket.connect((targetIP, port))
    except Exception as e:
        print(f"[-] Connection error: {e}")
        return

    # Get the initial response from the target, and then start loop
    try:
        banner = clientSocket.recv(4096)
        if banner:
            print(banner.decode(errors="replace"))
    except socket.timeout:
        # No banner sent
        pass

    clientSocket.settimeout(None)

    # Start basic connection loop
    basicClientConnectionLoop(clientSocket, targetIP)

    # Clean up and exit
    clientSocket.close()
    return


def fileClientSendFile(clientSocket, localFilename, remoteFilename):
    # Create a list of items where each item is a line of the local file
    localFiledata = []

    with open(localFilename, "r") as localFile:
        for line in localFile:
            localFiledata.append(line.strip())

    # Loop over each line of data
    # Wait to receive the READY code, then send a line of data
    for line in localFiledata:
        # Get the ready message from the server
        readyCodeResponse = clientSocket.recv(1024).decode()
        if readyCodeResponse != "READY":
            return

        # Send the data
        lineData = line.encode()
        clientSocket.send(lineData)

def fileTransferClientLoop(clientSocket):
    # Main connection loop where commands can be manually sent
    while True:
        command = input(f"{targetIP}> ")
        tokenList = commmand.split(" ")

        if len(tokenList) == 3 and tokenList[0].lower() == "upload":
            localFilename = tokenList[1]
            remoteFilename = tokenList[2]

            # Send request to upload a file
            # The request is made of the command and the remote filename
            uploadReqest = f"UPLOAD {remoteFilename}".encode()
            clientSocket.send(uploadRequest)

            # Check to see if the upload request was accepted
            uploadRequestResponse = clientSocket.recv(1024).decode()

            if uploadRequestResponse == "ACCEPT":
                fileClientSendFile(clientSocket, localFilename, remoteFilename)


def connectFileTransfer(targetIP, port):
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.settimeout(3)

    try:
        clientSocket.connect((targetIP, port))

    except Exception as e:
        print(f"[-] Error: {e}")
        return

    # Connection succeeded, start main loop with the client object
    fileTransferClientLoop(clientSocket)

def netkiwiClient(targetIP, port, mode):
    if mode.lower() == "http":
        connectHTTP(targetIP, port)
    elif mode.lower() == "file":
        connectFileTransfer(targetIP, port)
    
    else:
        connectBasic(targetIP, port)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 client.py <IP> <port> <mode>")
        exit(1)

    targetIP = sys.argv[1]
    port = int(sys.argv[2])
    mode = sys.argv[3].lower()

    netkiwiClient(targetIP, port, mode)
