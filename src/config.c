#ifndef __HAP_H


#include <hap.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEFAULT_CONFIG_EXTENSION ".ini"
#define HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE 16


typedef enum {
    NONE,
    SECTION,
    OPTION,
    FINISHED,
} HAPConfigurationTokenType;

typedef struct HAPConfigurationToken HAPConfigurationToken;


struct HAPConfigurationToken {
    HAPConfigurationTokenType type;
    char *value;
};


FILE* hap_configuration_file(HAPEngine *engine, char *fileName) {
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        (*engine).log_info(engine, "Failed to load configuration file: %s\n", fileName);
        free(fileName);
        return NULL;
    }

    return file;
}


HAPConfigurationToken* hap_configuration_token_next(FILE *file) {
    size_t valAllocatedSize;
    short valIndex;
    char cursor;

    HAPConfigurationToken *token;

    token = calloc(1, sizeof(HAPConfigurationToken));

    if (token == NULL) return NULL;

    (*token).type = NONE;
    valAllocatedSize = HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE;
    (*token).value = calloc(valAllocatedSize, sizeof(char));

    if ((*token).value == NULL) {
        free(token);
        return NULL;
    }

    if (feof(file)) {
        (*token).type = FINISHED;
        (*token).value = NULL;

        return token;
    }

    for (flockfile(file); !feof(file);) {
        cursor = getc_unlocked(file);

        if ((*token).type == SECTION && cursor == ']') break;
        else if ((*token).type == NONE && (cursor == ' ' || cursor == '\n')) continue;
        else if (cursor == '\n') break;

        if (cursor == '[') {
            valIndex = 0;

            (*token).type = SECTION;

            if ((*token).value == NULL) {
                funlockfile(file);
                free(token);
                return NULL;
            }

            continue;
        }

        if (valIndex >= valAllocatedSize) {
            valAllocatedSize = (HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE % valIndex) * HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE * sizeof(char);
            (*token).value = realloc((*token).value, valAllocatedSize);

            if ((*token).value == NULL) {
                free(token);
                funlockfile(file);
                return NULL;
            }
        }

        if ((*token).type == NONE) (*token).type = OPTION;

        (*token).value[valIndex] = cursor;
        ++valIndex;
    }

    funlockfile(file);
    return token;
}

char *hap_configuration_filename(HAPEngine *engine, char *identifier) {
    int fileNameLength;
    char *fileName;

    // TODO: Check for extension before concatenation?
    if (identifier == NULL) identifier = (*engine).name;

    fileNameLength = snprintf(NULL, 0, "%s%s", identifier, DEFAULT_CONFIG_EXTENSION);

    if (fileNameLength < 0) return NULL;

    fileName = calloc(fileNameLength, sizeof(char));

    if (fileName == NULL) return NULL;

    fileNameLength = snprintf(fileName, fileNameLength+1, "%s%s", identifier, DEFAULT_CONFIG_EXTENSION);

    if (fileNameLength < 0) {
        free(fileName);
        return NULL;
    }

    return fileName;
}

HapConfigurationSection* hap_configuration_process_section(HAPConfigurationToken *token) {
    HapConfigurationSection* section;
    section = calloc(1, sizeof(HapConfigurationSection));
    (*section).name = (*token).value;
    return section;
}

HapConfigurationOption* hap_configuration_process_option(HAPConfigurationToken *token) {
    size_t keyLength, valueLength;
    char *key, *value;

    HapConfigurationOption *option;

    option = calloc(1, sizeof(HapConfigurationOption));

    if (option == NULL) return NULL;

    key = strtok((*token).value, "=");
    value = strtok(NULL, "=");

    if (value == NULL) value = "true";

    keyLength = snprintf(NULL, 0, "%s", key);
    valueLength = snprintf(NULL, 0, "%s", value);

    (*option).key = calloc(keyLength, sizeof(char));

    if ((*option).key == NULL) return NULL;

    (*option).value = calloc(valueLength, sizeof(char));

    if ((*option).value == NULL) {
        free((*option).key);
        free(option);

        return NULL;
    }

    if ((*option).key == NULL) return NULL;
    if ((*option).value == NULL) return NULL;

    if (snprintf((*option).key, keyLength, "%s", key) == -1) {
        free(key);
        free(value);

        return NULL;
    }

    if (snprintf((*option).value, valueLength, "%s", value) == -1) {
        free(key);
        free(value);
        free((*option).key);
        free((*option).value);
        free(option);

        return NULL;
    }

    return option;
}

HapConfiguration* hap_configuration_load(HAPEngine *engine, char *identifier) {
    FILE *file;
    char *fileName;

    HapConfiguration *config;

    HAPConfigurationToken *token;
    HapConfigurationOption *option;
    HapConfigurationSection *section;

    section = NULL;

    config = calloc(1, sizeof(HapConfiguration));

    if (config == NULL) return NULL;

    (*config).totalGlobals = 0;
    (*config).globals = NULL;

    fileName = hap_configuration_filename(engine, identifier);

    if (fileName == NULL) return NULL;

    file = hap_configuration_file(engine, fileName);

    for (;;) {
        token = hap_configuration_token_next(file);

        if (token == NULL) {
            (*engine).log_error(
                engine,
                "Failed to load configuration file: %s",
                fileName
            );

            if (fileName != NULL) free(fileName);

            fileName = NULL;
            hap_configuration_destroy(config);

            return NULL;
        }

        if ((*token).type == FINISHED) {
            free(token);
            break;
        }

        if ((*token).type == SECTION) {
            ++(*config).totalSections;

            section = hap_configuration_process_section(token);

            (*config).sections = realloc((*config).sections, sizeof(HapConfigurationSection*) * (*config).totalSections);
            (*config).sections[(*config).totalSections-1] = section;

            if ((*config).sections[(*config).totalSections - 1] == NULL) {
                free(token);
                hap_configuration_destroy(config);
            }

        } else if ((*token).type == OPTION) {
            option = hap_configuration_process_option(token);

            if (section == NULL) {
                ++(*config).totalGlobals;

                (*config).globals = realloc((*config).globals, (*config).totalGlobals * sizeof(HapConfigurationOption*));
                (*config).globals[(*config).totalGlobals - 1] = option;

            } else {
                ++(*section).totalOptions;

                (*section).options = realloc((*section).options, (*section).totalOptions * sizeof(HapConfigurationOption*));
                (*section).options[(*section).totalOptions - 1] = option;
            }

        } else {
            (*engine).log_warning(engine, "Unknown token found in configuration file: %s\n", fileName);
        }

        free(token);
    }

    free(fileName);
    funlockfile(file);
    fclose(file);

    return config;
}


void hap_configuration_destroy(HapConfiguration *config) {
    free(config);
}


#define __HAP_H
#endif
