#include <hap.h>
#include <stdio.h>

#define DEFAULT_CONFIG_EXTENSION ".ini"
#define HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE 16
#define CHARACTER_IS_WHITESPACE(cursor) (cursor == ' ' || cursor == '\n' || cursor == '\r' || cursor == '\t' || cursor == '\0')


typedef enum {
    NONE,
    SECTION,
    OPTION,
    COMMENT,
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


int hap_configuration_token_next(FILE *file, HAPConfigurationToken *token) {
    char cursor;
    short valueIndex, allocatedValueSize;

    valueIndex = 0;
    allocatedValueSize = HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE;

    (*token).type = NONE;
    (*token).value = calloc(allocatedValueSize, sizeof(char));

    if ((*token).value == NULL) {
        return 1;
    }

    while (!feof(file)) {
        cursor = getc(file);

        if (cursor == -1) {
            (*token).type = FINISHED;
            (*token).value = "";
            break;

        } else if ((*token).type == NONE) {
            if CHARACTER_IS_WHITESPACE(cursor) {
                continue;

            } else if (cursor == '[' || cursor == '!') {
                (*token).type = SECTION;
                continue;

            } else if (cursor == '#') {
                (*token).type = COMMENT;
                continue;

            } else {
                (*token).type = OPTION;
            }

        } else if ((*token).type == SECTION && (cursor == ']' || cursor == '\n')) {
            break;

        } else if ((*token).type == OPTION && cursor == '\n') {
            break;
        }

        if (valueIndex == allocatedValueSize-1) {
            // Grow option buffer by ALLOCATED_TOKEN_VALUE_BUFFER_SIZE
            allocatedValueSize = (valueIndex % HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE ) * HAP_CONFIG_TOKEN_VALUE_BUFFER_SIZE * sizeof(char);
            (*token).value = realloc((*token).value, allocatedValueSize);
            if ((*token).value == NULL) return 1;
        }


        if (valueIndex == 0 && CHARACTER_IS_WHITESPACE(cursor))
            continue;

        (*token).value[valueIndex] = cursor;
        ++valueIndex;
    }

    return 0;
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

HAPConfigurationSection* hap_configuration_process_section(HAPConfigurationToken *token) {
    HAPConfigurationSection* section;
    section = calloc(1, sizeof(HAPConfigurationSection));
    (*section).name = (*token).value;
    return section;
}

HAPConfigurationOption* hap_configuration_process_option(HAPConfigurationToken *token) {
    size_t keyLength, valueLength;
    char *key, *value;

    HAPConfigurationOption *option;

    option = calloc(1, sizeof(HAPConfigurationOption));

    if (option == NULL) return NULL;

    key = strtok((*token).value, "=");
    value = strtok(NULL, "=");

    if (value == NULL) value = "true";

    keyLength = snprintf(NULL, 0, "%s", key);
    valueLength = snprintf(NULL, 0, "%s", value);

    (*option).key = calloc(keyLength+1, sizeof(char));

    if ((*option).key == NULL) return NULL;

    (*option).value = calloc(valueLength+1, sizeof(char));

    if ((*option).value == NULL) {
        free((*option).key);
        free(option);

        return NULL;
    }

    if ((*option).key == NULL) return NULL;
    if ((*option).value == NULL) return NULL;

    if (snprintf((*option).key, keyLength+1, "%s", key) == -1) {
        free(key);
        free(value);

        return NULL;
    }

    if (snprintf((*option).value, valueLength+1, "%s", value) == -1) {
        free(key);
        free(value);
        free((*option).key);
        free((*option).value);
        free(option);

        return NULL;
    }

    return option;
}

HAPConfiguration* hap_configuration_load(HAPEngine *engine, char *identifier) {
    FILE *file;
    char *fileName;

    HAPConfiguration *config;

    HAPConfigurationToken token;
    HAPConfigurationOption *option;
    HAPConfigurationSection *section;

    section = NULL;

    config = calloc(1, sizeof(HAPConfiguration));

    if (config == NULL) return NULL;

    (*config).totalGlobals = 0;
    (*config).globals = NULL;

    fileName = hap_configuration_filename(engine, identifier);

    (*engine).log_debug(engine, "Loading configuration file: %s", fileName);

    if (fileName == NULL) return NULL;

    file = hap_configuration_file(engine, fileName);

    if (file == NULL) return NULL;

    int error = 0;

    for (;;) {
        error = hap_configuration_token_next(file, &token);

        (*engine).log_debug(
            engine, "config\t%s\t%d\t%s",
            fileName, (token).type, token.value
        );

        if (error != 0) {
            (*engine).log_error(
                engine,
                "Failed to load token in: %s",
                fileName
            );

            if (fileName != NULL) free(fileName);

            fileName = NULL;
            hap_configuration_destroy(config);

            return NULL;
        }

        if (token.type == FINISHED) {
            break;
        }

        if (token.type == SECTION) {
            ++(*config).totalSections;

            section = hap_configuration_process_section(&token);

            (*config).sections = realloc((*config).sections, (*config).totalSections * sizeof(HAPConfigurationSection*));
            (*config).sections[(*config).totalSections-1] = section;

            if ((*config).sections[(*config).totalSections - 1] == NULL) {
                hap_configuration_destroy(config);
            }

        } else if (token.type == OPTION) {
            option = hap_configuration_process_option(&token);

            if (section == NULL) {
                ++(*config).totalGlobals;

                (*config).globals = realloc((*config).globals, (*config).totalGlobals * sizeof(HAPConfigurationOption*));
                (*config).globals[(*config).totalGlobals - 1] = option;

            } else {
                ++(*section).totalOptions;

                (*section).options = realloc((*section).options, (*section).totalOptions * sizeof(HAPConfigurationOption*));
                (*section).options[(*section).totalOptions - 1] = option;
            }

        } else {
            (*engine).log_warning(engine, "Unknown token found in configuration file: %s\n", fileName);
        }
    }

    free(fileName);
    fclose(file);

    return config;
}


void hap_configuration_destroy(HAPConfiguration *config) {
    free(config);
}
