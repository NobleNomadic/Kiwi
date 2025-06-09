from impacket.dcerpc.v5 import transport
import impacket.uuid as impuuid
import sys

# Check if a target IP is vulnerable to MS08-067
def checkXPOverflow(targetIP):
    print(f"[*] Checking {targetIP} for XP-Overflow vulnerability")

    stringBinding = f"ncacn_np:{targetIP}[\\pipe\\browser]"

    try:
        trans = transport.DCERPCTransportFactory(stringBinding)
        trans.set_connect_timeout(5)
        dce = trans.get_dce_rpc()
        dce.connect()
        print("[*] SMB connection established")

        # UUID for the Server Service (srvsvc)
        serverUUID = uuid.uuidtup_to_bin(('4b324fc8-1670-01d3-1278-5a47bf6ee188', '3.0'))
        dce.bind(serverUUID)
        print("[+] Target is likely vulnerable")

        dce.disconnect()
        return True

    except Exception as e:
        print("[-] Target is likely patched or not vulnerable")
        print(f"[-] Error: {str(e)}")
        return False

if __name__ == "__main__":
    if len(sys.argv) != 2:
        exit(1)

    checkXPOverflow(sys.argv[1])
