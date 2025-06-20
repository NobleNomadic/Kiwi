#!/usr/bin/python3
import dns.resolver
import random
import sys

def findSubdomains(targetDomain, wordlistPath):
    dnsResolver = dns.resolver.Resolver()
    dnsResolver.timeout = 2
    dnsResolver.lifetime = 2

    with open(wordlistPath, "r") as file:
        subdomainList = file.read().splitlines()
    random.shuffle(subdomainList)

    print(f"[*] Starting subdomain scan on: {targetDomain}")

    foundDomains = []

    for sub in subdomainList:
        fullDomain = f"{sub}.{targetDomain}"
        try:
            answers = dnsResolver.resolve(fullDomain, "A")
            for ip in answers:
                print(f"[+] {fullDomain:<30} -> {ip}")
                # Store the output in an array of objects
                foundDomains.append({"domain": fullDomain, "ip": str(ip)})

        except dns.resolver.NXDOMAIN:
            pass
        except dns.resolver.NoAnswer:
            pass
        except dns.resolver.Timeout:
            print(f"[-] Timeout on {fullDomain}")
        except Exception as error:
            print(f"[-] Error on {fullDomain}: {error}")

    print(f"[*] Found {len(foundDomains)} subdomains")

    returnData = {
        "url": targetDomain,
        "found": foundDomains
    }
    return returnData

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 subdomainfind.py <domain> <wordlist>")
        sys.exit(1)

    result = findSubdomains(sys.argv[1], sys.argv[2])