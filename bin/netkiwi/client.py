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
            break

    print(response.decode(errors="replace"))
    clientSocket.close()
    return

def clientConnectionLoop(clientSocket, targetIP):
    while True:
        try:
            command = input(f"{targetIP}> ")
            if command.lower() in ('exit', 'quit'):
                break

            clientSocket.send(command.encode() + b'\n')

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
    clientConnectionLoop(clientSocket, targetIP)

    # Clean up and exit
    clientSocket.close()
    return

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 client.py <IP> <port> <basic|http>")
        exit(1)

    ip = sys.argv[1]
    port = int(sys.argv[2])
    mode = sys.argv[3].lower()

    if mode == "http":
        connectHTTP(ip, port)
    else:
        connectBasic(ip, port)
