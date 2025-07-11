#!/bin/bash
# Build script to compile all tools
cd recon/portscanner
gcc -o portscanner portscanner.c
cd ../dnsfind
gcc -o dnsfind dnsfind.c
cd ../webfind
gcc -o webfind webfind.c -lcurl
cd ../../exploit/packer
gcc -o packer packer.c
cd ../../utility/bruteclaw
gcc -o bruteclaw bruteclaw.c -lcurl -lssh
cd ../dnstool
gcc -o dnstool dnstool.c
cd ../netcatcher
gcc -o netcatcher netcatcher.c -lpcap
cd ../sshclient
gcc -o sshclient sshclient.c -lssh
