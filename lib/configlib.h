/*
 *  Kiwi - Penetration testing toolkit
 *
 * Copyright (C) 2025 NobleNomadic
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef configlib_h
#define configlib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Config file structure
// The first line is a single int header
// The second line is a list of ints
typedef struct {
    int header;
    int *intArray;
    size_t intCount;
} configFileData;

configFileData loadConfigFile(const char *configFilename) {
    configFileData configData = {0, NULL, 0};
    FILE *configFile = fopen(configFilename, "r");
    if (!configFile) {
        fprintf(stderr, "Error: Could not open config file '%s'\n", configFilename);
        return configData;
    }

    char lineBuffer[1024];

    // Read header line
    if (fgets(lineBuffer, sizeof(lineBuffer), configFile)) {
        configData.header = atoi(lineBuffer);
    }

    // Temporary array to hold parsed ports
    int *tempArray = NULL;
    size_t count = 0;

    // Read all remaining lines
    while (fgets(lineBuffer, sizeof(lineBuffer), configFile)) {
        char *tok = strtok(lineBuffer, " \t\n\r");
        while (tok) {
            int port = atoi(tok);
            // Reallocate array to store new port
            int *newArray = realloc(tempArray, (count + 1) * sizeof(int));
            if (!newArray) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                free(tempArray);
                fclose(configFile);
                return configData;
            }
            tempArray = newArray;
            tempArray[count++] = port;
            tok = strtok(NULL, " \t\n\r");
        }
    }

    fclose(configFile);

    configData.intArray = tempArray;
    configData.intCount = count;

    return configData;
}

// Free dynamically allocated data
void freeConfig(configFileData *cfg) {
    if (cfg->intArray) {
        free(cfg->intArray);
        cfg->intArray = NULL;
        cfg->intCount = 0;
    }
}

#endif
