import socket
import sys
import os
import time

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

def fileClientSendFile(clientSocket, localFilename, remoteFilename):
    if not os.path.exists(localFilename):
        print(f"[-] File {localFilename} not found.")
        return False

    try:
        with open(localFilename, "r") as f:
            lines = [line.rstrip('\r\n') for line in f]  # Remove only line endings, preserve blank lines

        print(f"[*] Sending {len(lines)} lines...")
        
        for i, line in enumerate(lines):
            try:
                # Wait for READY signal
                clientSocket.settimeout(15)
                ready = clientSocket.recv(1024).decode().strip()
                print(f"[DEBUG] Received from server: '{ready}'")
                
                if ready == "READY":
                    # Handle blank lines by sending a special marker
                    if line == "":
                        line_to_send = "//:BLANKLINE"
                        display_line = "<BLANK LINE>"
                    else:
                        line_to_send = line
                        display_line = line[:50] + ('...' if len(line) > 50 else '')
                    
                    line_data = line_to_send.encode('utf-8')
                    clientSocket.send(line_data)
                    print(f"[*] Sent line {i+1}/{len(lines)}: '{display_line}'")
                else:
                    print(f"[-] Server responded with '{ready}' instead of READY.")
                    return False
            except socket.timeout:
                print(f"[-] Timeout waiting for server READY signal at line {i+1}.")
                return False
            except Exception as e:
                print(f"[-] Error sending line {i+1}: {e}")
                return False

        # Wait for final READY signal and send end transfer
        try:
            clientSocket.settimeout(15)
            ready = clientSocket.recv(1024).decode().strip()
            print(f"[DEBUG] Final READY signal: '{ready}'")
            
            if ready == "READY":
                print("[*] Sending end transfer signal...")
                clientSocket.send("//:ENDTRANSFER".encode('utf-8'))
            else:
                print(f"[-] Expected final READY, got: {ready}")
                return False
        except socket.timeout:
            print("[-] Timeout waiting for final READY signal.")
            return False
        
        # Wait for final confirmation
        try:
            clientSocket.settimeout(15)
            final = clientSocket.recv(1024).decode()
            print(f"[+] Upload result: {final}")
            return "FINISH" in final or "SUCCESS" in final
        except socket.timeout:
            print("[-] Timeout waiting for final confirmation.")
            return False
        
    except Exception as e:
        print(f"[-] Error during file transfer: {e}")
        return False
    finally:
        clientSocket.settimeout(None)


def fileTransferClientLoop(clientSocket, targetIP):
    print("[*] File transfer mode. Commands: upload <localfile> <remotefile>, quit")
    
    while True:
        try:
            command = input(f"{targetIP}> ")
            
            if command.lower() in ("exit", "quit"):
                print("[*] Closing connection...")
                break
                
            tokenList = command.strip().split(" ")
            
            if len(tokenList) == 3 and tokenList[0].lower() == "upload":
                localFilename = tokenList[1]
                remoteFilename = tokenList[2]
                
                print(f"[*] Requesting upload: {localFilename} -> {remoteFilename}")
                uploadRequest = f"UPLOAD {remoteFilename}".encode()
                clientSocket.send(uploadRequest)
                
                # Wait for server response
                clientSocket.settimeout(10)
                response = clientSocket.recv(1024).decode().strip()
                clientSocket.settimeout(None)
                
                if response == "ACCEPT":
                    print("[*] Server accepted upload, starting transfer...")
                    success = fileClientSendFile(clientSocket, localFilename, remoteFilename)
                    if success:
                        print("[+] File transfer completed successfully")
                    else:
                        print("[-] File transfer failed")
                else:
                    print(f"[-] Upload request denied by server: {response}")
                    
            elif len(tokenList) == 1 and tokenList[0].lower() == "help":
                print("Available commands:")
                print("  upload <localfile> <remotefile> - Upload a file to the server")
                print("  quit/exit - Close connection")
                print("  help - Show this help message")
                
            else:
                print("Usage: upload <localfile> <remotefile>")
                print("Type 'help' for more commands")
                
        except KeyboardInterrupt:
            print("\n[*] Interrupted by user")
            break
        except Exception as e:
            print(f"[-] Error in file transfer loop: {e}")
            break

def connectFileTransfer(targetIP, port):
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.settimeout(10)
    
    try:
        print(f"[*] Connecting to {targetIP}:{port} for file transfer...")
        clientSocket.connect((targetIP, port))
        print(f"[+] Connected to {targetIP}:{port}")
        
        # Try to receive banner/welcome message
        try:
            clientSocket.settimeout(3)
            banner = clientSocket.recv(1024)
            if banner:
                print(f"[*] Server: {banner.decode(errors='replace').strip()}")
        except socket.timeout:
            print("[*] No banner received from server")
        
        clientSocket.settimeout(None)
        fileTransferClientLoop(clientSocket, targetIP)
        
    except Exception as e:
        print(f"[-] Connection error: {e}")
    finally:
        try:
            clientSocket.close()
        except:
            pass

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
        print("Modes: http, file, basic")
        sys.exit(1)

    targetIP = sys.argv[1]
    port = int(sys.argv[2])
    mode = sys.argv[3].lower()
    
    if mode not in ["http", "file", "basic"]:
        print("[-] Invalid mode. Use: http, file, or basic")
        sys.exit(1)
        
    netkiwiClient(targetIP, port, mode)
