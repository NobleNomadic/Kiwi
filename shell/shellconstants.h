// shellconstants.h - Constants for kiwishell
#ifndef shellconstants_h
#define shellconstants_h

// Colours
#define ANSI_FG_RED       "\x1b[31m"
#define ANSI_FG_GREEN     "\x1b[32m"
#define ANSI_FG_YELLOW    "\x1b[33m"
#define ANSI_FG_BLUE      "\x1b[34m"
#define ANSI_FG_WHITE     "\x1b[37m"
#define ANSI_FG_DEFAULT   "\x1b[39m"
#define ANSI_RESET        "\x1b[0m"

// Main banner
const char *kiwiShellBanner =
"    __ __ _____       ______       _____ __  __________    __\n"
"   / //_//  _/ |     / /  _/      / ___// / / / ____/ /   / /\n"
"  / ,<   / / | | /| / // /        \\__ \\/ /_/ / __/ / /   / /\n"
" / /| |_/ /  | |/ |/ // /        ___/ / __  / /___/ /___/ /___\n"
"/_/ |_/___/  |__/|__/___/       /____/_/ /_/_____/_____/_____/\n";

// General commands menu
const char *kiwiCommandsMenu = ANSI_FG_BLUE
"[   RECON   ]   [  EXPLOIT  ]    [  PAYLOAD  ]   [  UTILITY  ]\n" ANSI_RESET
"├ portscanner   ├ webdrop        ├ sqli          ├ bruteclaw\n"
"├ dnsfind       └ packer         └ rshell        ├ dnstool\n"
"└ webfind                                        ├ sshclient\n"
"                                                 └ netcatcher\n";

#endif
