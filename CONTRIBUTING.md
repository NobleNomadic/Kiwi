# Contributing to Kiwi
The kiwi repository is meant for everyone to add their own tools. Even if you are a beginner and are just looking for feedback, please feel welcome to use this repository to house your tools.
Even simply using kiwi as a place to store your tool's code while you develop it can help with finding feedback.

## Folder Structure
If you want to add your own tool to kiwi, create a new folder in the bin (binary) folder for it. If you want any configuration files for your tool, add them in the etc folder. You might also want to add logging systems. If this is the case, it recommended you output the logs of your tool to a JSON file in the log folder.
For any static data relating to your tool, you can use the share folder. Non-static data such as automation scripts for other tools can go in the data folder.
