#!/usr/bin/python3
from scapy.all import *

def inspectPacket(packet):
    print(packet.summary())

def sniffNetwork(count=10, filter=None):
    print(f"[*] Monitroing network traffic for {count} packets")

    if filter == None:
        packetData =sniff(prn=inspectPacket, count=count)
        return packetData

    print(f"[*] Using filter: {filter}")
    packetData = sniff(prn=inspectPacket, count=count, filter=filter)
    return packetData

if __name__ == "__main__":
    sniffNetwork(10000000)