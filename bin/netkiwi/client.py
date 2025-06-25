import socket
import sys
import os

def connectHTTP(targetIP, port):
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.settimeout(3)
    try:
        clientSocket.connect((targetIP, port))
        print(f"[+] Connected to {targetIP}:{port}")
        request = (
            f"GET / HTTP/1.1\r\n"
            f"Host: {targetIP}\r\n"
            "User-Agent: SimplePythonClient/1.0\r\n"
            "Connection: close\r\n\r\n"
        )
        clientSocket.send(request.encode())
        response = b""
        while True:
            chunk = clientSocket.recv(4096)
            if not chunk:
                break
            response += chunk
        print(response.decode(errors="replace"))
    except Exception as e:
        print(f"[-] Connection error: {e}")
    finally:
        clientSocket.close()

def basicClientConnectionLoop(clientSocket, targetIP):
    while True:
        try:
            command = input(f"{targetIP}> ")
            if command.lower() in ("exit", "quit"):
                break
            clientSocket.send(command.encode())
            response = clientSocket.recv(4096)
            if not response:
                print("[*] Connection closed by remote host")
                break
            print(response.decode(errors="replace"))
        except Exception as e:
            print(f"[-] Error: {e}")
            break

def connectBasic(targetIP, port):
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.settimeout(3)
    try:
        clientSocket.connect((targetIP, port))
        banner = clientSocket.recv(4096)
        if banner:
            print(banner.decode(errors="replace"))
        clientSocket.settimeout(None)
        basicClientConnectionLoop(clientSocket, targetIP)
    except Exception as e:
        print(f"[-] Connection error: {e}")
    finally:
        clientSocket.close()

def fileClientSendFile(clientSocket, localFilename, remoteFilename):
    if not os.path.exists(localFilename):
        print(f"[-] File {localFilename} not found.")
        return

    with open(localFilename, "r") as f:
        lines = [line.strip() for line in f.readlines()]

    for line in lines:
        ready = clientSocket.recv(1024).decode()
        if ready == "READY":
            clientSocket.send(line.encode())
        else:
            print("[-] Server did not respond with READY.")
            return

    clientSocket.send(b"//:ENDTRANSFER")
    final = clientSocket.recv(1024).decode()
    print(final)

def fileTransferClientLoop(clientSocket, targetIP):
    while True:
        command = input(f"{targetIP}> ")
        tokenList = command.split(" ")

        if len(tokenList) == 3 and tokenList[0].lower() == "upload":
            localFilename = tokenList[1]
            remoteFilename = tokenList[2]
            uploadRequest = f"UPLOAD {remoteFilename}".encode()
            clientSocket.send(uploadRequest)

            response = clientSocket.recv(1024).decode()
            if response == "ACCEPT":
                fileClientSendFile(clientSocket, localFilename, remoteFilename)
                break
            else:
                print("[-] Upload request denied.")
        else:
            print("Usage: upload <localfile> <remotefile>")

def connectFileTransfer(targetIP, port):
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        clientSocket.connect((targetIP, port))
        fileTransferClientLoop(clientSocket, targetIP)
    except Exception as e:
        print(f"[-] Connection error: {e}")
    finally:
        clientSocket.close()

def netkiwiClient(targetIP, port, mode):
    if mode == "http":
        connectHTTP(targetIP, port)
    elif mode == "file":
        connectFileTransfer(targetIP, port)
    else:
        connectBasic(targetIP, port)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 client.py <IP> <port> <mode>")
        sys.exit(1)

    targetIP = sys.argv[1]
    port = int(sys.argv[2])
    mode = sys.argv[3].lower()
    netkiwiClient(targetIP, port, mode)