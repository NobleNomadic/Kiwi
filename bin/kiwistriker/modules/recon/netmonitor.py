from scapy.all import *

def inspectPacket(packet):
    pass

def sniffNetwork(count):
    sniff(prn=inspectPacket, count=count)

if __name__ == "__main__":
    count = 1
    sniffNetwork(count)