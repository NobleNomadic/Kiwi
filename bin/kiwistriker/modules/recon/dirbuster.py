#!/usr/bin/python3
import requests

def findDirectories(url, wordlist):
    returnData = {
        "url": url,
        "found": []
    }

    if not url.endswith("/"):
        url += "/"

    with open(wordlist, "r") as f:
        words = f.read().splitlines()

    print(f"[*] Scanning {url} with {len(words)} entries")

    for word in words:
        target = url + word
        try:
            r = requests.get(target, timeout=5)
            if r.status_code not in [404]:
                print(f"[{r.status_code}] {target}")

                newPath = {
                    "url": target,
                    "status": r.status_code
                }

                returnData["found"].append(newPath)

        except requests.exceptions.RequestException:
            continue

    return returnData