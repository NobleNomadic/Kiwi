# Contributing
Everyone is welcome to contribute to kiwi to either add their own tool, or help develop a current one.

## Where do I put my tool?
If you want to add your own tool to kiwi, create a new folder in the bin (binary) folder for it. If you want any configuration files for your tool, add them in the etc folder. You might also want to add logging systems. If this is the case, it recommended you output the logs of your tool to a JSON file in the log folder.
For any static data relating to your tool, you can use the share folder. Non-static data such as automation scripts for other tools can go in the data folder.

## Contributing to current tools
If you are interested in adding to the current tools, please see the following list on things you can do to help kiwi.
- Adding new modes to netkiwi for the server and client, some ideas are below
	- Reverse shell listener
	- File transfer system
	- Chat server and client
- Create new exploits for kiwistriker
- Create example programs or systems that are vulnerable for testing tools
	- A server that has buffer overflow vulnerabilities
	- A guide on setting up an SSH server for testing kiwistrikers SSH brute forcer
- Create a guide or example usage of different tools in kiwi