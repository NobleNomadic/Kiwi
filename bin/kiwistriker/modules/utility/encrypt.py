#!/usr/bin/python3
import sys

def xorData(data, keyBytes):
    return bytes([b ^ keyBytes[i % len(keyBytes)] for i, b in enumerate(data)])


def xorFile(infile, outfile, key):
    keyBytes = key.encode('ascii')
    with open(infile, 'rb') as f:
        data = f.read()
    encrypted = xorData(data, keyBytes)
    with open(outfile, 'wb') as f:
        f.write(encrypted)
    print(f"[+] File data XORed and saved to {outfile}")


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 encrypt.py <infile> <outfile> <key>")
        exit(1)

    infile = sys.argv[1]
    outfile = sys.argv[2]
    key = sys.argv[3]

    xorFile(infile, outfile, key)