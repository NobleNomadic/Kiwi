#!/usr/bin/python3
import requests
import argparse

def findSubdomains(url, wordlist):
    if not url.endswith("/"):
        url += "/"

    with open(wordlist, "r") as f:
        words = f.read().splitlines()

    print(f"[*] Scanning on {url} with {len(words)} entries")

    for word in words:
        target = url + word
        try:
            r = requests.get(target, timeout=5)
            if r.status_code not in [404]:
                print(f"[{r.status_code}] {target}")
        except requests.exceptions.RequestException:
            continue