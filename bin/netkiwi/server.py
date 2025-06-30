import socket
import threading
import os
import sys
import time

def echoHandleClient(clientSocket, clientAddress):
    print(f"[+] Connection from {clientAddress}")
    try:
        data = clientSocket.recv(1024)
        print(f"[+] Data received from {clientAddress}: {data.decode()}")
        clientSocket.send(data)
    except Exception as e:
        print(f"[-] Error: {e}")
    finally:
        print(f"[+] Closing connection from {clientAddress}")
        clientSocket.close()

def echoServerListen(port):
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listener.bind(("0.0.0.0", port))
    listener.listen(5)
    print(f"[*] Echo server listening on port {port}")

    while True:
        try:
            clientSocket, clientAddress = listener.accept()
            threading.Thread(target=echoHandleClient, args=(clientSocket, clientAddress)).start()
        except KeyboardInterrupt:
            print("\n[*] Echo server shutting down...")
            break
        except Exception as e:
            print(f"[-] Error accepting connection: {e}")
    
    listener.close()

def receiveFileData(clientSocket):
    """Receive file data from client using the READY/send protocol"""
    fileData = []
    line_count = 0
    try:
        clientSocket.settimeout(30)  # Increased timeout for file transfers
        
        while True:
            # Send READY signal to client
            clientSocket.send(b"READY")
            timestamp = time.strftime("%H:%M:%S")
            print(f"[DEBUG {timestamp}] Sent READY signal for line {line_count + 1}")
            
            # Receive chunk from client
            try:
                raw_chunk = clientSocket.recv(1024)
                if not raw_chunk:  # Check raw bytes, not decoded string
                    print("[DEBUG] No data received, connection may be closed")
                    break
                
                chunk = raw_chunk.decode('utf-8', errors='replace')
                chunk_stripped = chunk.strip()
                print(f"[DEBUG] Got chunk: '{chunk}' (stripped: '{chunk_stripped}')")
                
                # Check for end transfer signal
                if chunk_stripped == "//:ENDTRANSFER":
                    print("[DEBUG] End transfer signal received")
                    break
                
                # Check for blank line marker
                if chunk_stripped == "//:BLANKLINE":
                    print("[DEBUG] Blank line marker received")
                    fileData.append("\n")  # Add actual blank line
                    line_count += 1
                    print(f"[DEBUG] Added blank line {line_count} to file data")
                else:
                    # Add the line to file data (preserve original formatting)
                    line_to_add = chunk.rstrip('\r\n') + "\n"
                    fileData.append(line_to_add)
                    line_count += 1
                    print(f"[DEBUG] Added line {line_count} to file data: '{line_to_add.rstrip()}'")
                
            except socket.timeout:
                print("[-] Timeout waiting for data from client")
                break
            except UnicodeDecodeError as e:
                print(f"[-] Unicode decode error: {e}")
                break
                
    except Exception as e:
        print(f"[-] Error in receiveFileData: {e}")
    finally:
        clientSocket.settimeout(None)  # Reset timeout
    
    print(f"[DEBUG] Received {len(fileData)} lines total")
    return fileData


def fileHandleClient(clientSocket, clientAddress):
    print(f"[+] File server connection from {clientAddress}")
    
    # Create files directory if it doesn't exist
    try:
        os.makedirs("files", exist_ok=True)
    except Exception as e:
        print(f"[-] Error creating files directory: {e}")
        clientSocket.close()
        return
    
    try:
        clientSocket.settimeout(10)  # Initial timeout for command
        
        while True:
            try:
                # Wait for client command
                data = clientSocket.recv(1024)
                if not data:
                    print(f"[*] Client {clientAddress} disconnected")
                    break
                    
                command = data.decode('utf-8', errors='replace').strip()
                print(f"[+] Client request from {clientAddress}: {command}")
                
                tokens = command.split(" ")
                
                if len(tokens) == 2 and tokens[0].upper() == "UPLOAD":
                    remoteFilename = tokens[1]
                    
                    # Validate filename (basic security check)
                    if not remoteFilename or ".." in remoteFilename or "/" in remoteFilename or "\\" in remoteFilename:
                        print(f"[-] Invalid filename: {remoteFilename}")
                        clientSocket.send(b"DENY")
                        continue
                    
                    # Accept the upload
                    clientSocket.send(b"ACCEPT")
                    print(f"[*] Accepting file upload: {remoteFilename} from {clientAddress}")
                    
                    # Receive file data
                    fileData = receiveFileData(clientSocket)
                    
                    if fileData:
                        try:
                            # Write file to disk
                            filepath = f"files/{remoteFilename}"
                            with open(filepath, "w", encoding='utf-8') as f:
                                f.writelines(fileData)
                            
                            clientSocket.send(b"FINISH")
                            print(f"[+] Successfully saved {len(fileData)} lines to {filepath}")
                            
                        except Exception as e:
                            print(f"[-] Error writing file: {e}")
                            clientSocket.send(b"ERROR")
                    else:
                        print("[-] No file data received")
                        clientSocket.send(b"ERROR")
                        
                elif len(tokens) == 1 and tokens[0].upper() == "QUIT":
                    print(f"[*] Client {clientAddress} requested disconnect")
                    clientSocket.send(b"GOODBYE")
                    break
                    
                elif len(tokens) == 1 and tokens[0].upper() == "LIST":
                    # List files in the files directory
                    try:
                        files = os.listdir("files")
                        if files:
                            file_list = "\n".join(files)
                            clientSocket.send(f"FILES:\n{file_list}".encode())
                        else:
                            clientSocket.send(b"NO_FILES")
                    except Exception as e:
                        print(f"[-] Error listing files: {e}")
                        clientSocket.send(b"ERROR")
                        
                else:
                    print(f"[-] Invalid command from {clientAddress}: {command}")
                    clientSocket.send(b"DENY")
                    
            except socket.timeout:
                print(f"[-] Timeout waiting for command from {clientAddress}")
                break
            except Exception as e:
                print(f"[-] Error handling client command: {e}")
                break
                
    except Exception as e:
        print(f"[-] Error in fileHandleClient: {e}")
    finally:
        try:
            clientSocket.close()
        except:
            pass
        print(f"[+] Closed connection from {clientAddress}")

def fileServerListen(port):
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        listener.bind(("0.0.0.0", port))
        listener.listen(5)
        print(f"[*] File server listening on port {port}")
        print(f"[*] Files will be saved to: {os.path.abspath('files')}")

        while True:
            try:
                clientSocket, clientAddress = listener.accept()
                print(f"[*] New file server connection from {clientAddress}")
                
                # Start client handler in new thread
                client_thread = threading.Thread(
                    target=fileHandleClient, 
                    args=(clientSocket, clientAddress),
                    daemon=True
                )
                client_thread.start()
                
            except KeyboardInterrupt:
                print("\n[*] File server shutting down...")
                break
            except Exception as e:
                print(f"[-] Error accepting connection: {e}")
                
    except Exception as e:
        print(f"[-] Error starting file server: {e}")
    finally:
        try:
            listener.close()
        except:
            pass

def setupServer(port, mode):
    print(f"[*] Starting server on port {port} in {mode} mode")
    
    if mode == "echo":
        echoServerListen(port)
    elif mode == "file":
        fileServerListen(port)
    else:
        print("[-] Invalid mode. Use 'echo' or 'file'")
        return False
    
    return True

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 server.py <port> <mode>")
        print("Modes:")
        print("  echo - Simple echo server")
        print("  file - File transfer server")
        sys.exit(1)
    
    try:
        port = int(sys.argv[1])
        if port < 1 or port > 65535:
            print("[-] Port must be between 1 and 65535")
            sys.exit(1)
    except ValueError:
        print("[-] Port must be a valid integer")
        sys.exit(1)
    
    mode = sys.argv[2].lower()
    
    if mode not in ["echo", "file"]:
        print("[-] Invalid mode. Use 'echo' or 'file'")
        sys.exit(1)
    
    try:
        setupServer(port, mode)
    except KeyboardInterrupt:
        print("\n[*] Server stopped by user")
    except Exception as e:
        print(f"[-] Server error: {e}")
