#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "cli.h"

// Recursively creates entities with names from [lineage]
// with [startingParent] as the first parent
// Trying to add an existing entity is ignored
// Example: add(world, {"forest", "tree", NULL});
static Status add(Entity startingParent, Lineage lineage) {
    if (*lineage == NULL) {
        return OK;
    } else {
        Entity nextParent = findValueByName(startingParent->childrenEntities,
                                            *lineage);
        if (nextParent == NULL)
            nextParent = createEntity(startingParent, *lineage);
        if (nextParent == NULL)
            return MALLOC_ERROR;
        return add(nextParent, lineage + 1);
    }
}

// Finds an entity using its [lineage], starting from [world]
// and returns it or returns NULL if not found
static Entity findDescendant(Entity startingParent, Lineage lineage) {
    if (*lineage == NULL || startingParent == NULL) {
        return startingParent;
    } else {
        Entity nextParent = findValueByName(startingParent->childrenEntities,
                                            *lineage);
        return findDescendant(nextParent, lineage + 1);
    }
}

// Finds an entity using its [lineage], starting from [world]
// and deletes it if it exists along with all of its descendants
// Example: delete(world, {"forest", "tree", NULL});
static Status delete(Entity world, Lineage lineage) {
    if (*lineage == NULL) {
        // Special case
        // Do not delete [world] itself, but all of its descendants
        // Otherwise, in the else block, [toBeDeleted] would be equal to [world]
        freeAllDescendants(world);
    } else {
        Entity toBeDeleted = findDescendant(world, lineage);
        disownEntity(toBeDeleted);
        freeEntity(toBeDeleted);
    }
    return OK;
}

// Prints the name of [entity] to stdout
static void printEntity(Entity entity) {
    printf("%s\n", entity->name);
}

// Finds an entity using its [lineage], starting from [world]
// and prints all of its children
// Example: print(world, {"forest", "tree", NULL});
static Status print(Entity world, Lineage lineage) {
    Entity toBePrinted = findDescendant(world, lineage);
    if (toBePrinted != NULL)
        forEachValue(toBePrinted->childrenEntities, &printEntity);
    return NO_STATUS;
}

// Tries to find an entity using its [lineage], starting from [startingParent]
// Returns YES if found or NO otherwise
// "*" is allowed as a wildcard
// Example: check(world, {"forest", "*", "animal", NULL});
static Status check(Entity startingParent, Lineage lineage) {
    if (startingParent == NULL) {
        return NO;
    } else if (*lineage == NULL) {
        return YES;
    } else {
        Status status = NO;

        // Checks [nextInLine], a child of [startingParent]
        // as a candidate for the next startingParent
        void checkNextInLine(Entity nextInLine) {
            if (status == NO)
                status = check(nextInLine, lineage + 1);
        }

        if (strcmp(*lineage, "*") == 0) {
            // All children of [startingParent] are candidates for the next
            // startingParent
            forEachValue(startingParent->childrenEntities, &checkNextInLine);

        } else {
            // There's only one candidate for the next startingParent
            Entity nextParent = findValueByName(startingParent->childrenEntities,
                                                *lineage);
            checkNextInLine(nextParent);
        }
        return status;
    }
}

// Split [command] into tokens
// Saves it into a NULL-terminated array [tokens]
// Modifies [command]
static void parse(Command command, char **tokens) {
    for (int i = 0; i < MAX_NUMBER_OF_ARGUMENTS + 1 && *command != '\0'; i++) {
        // Skip all whitespace characters
        while (*command != '\0' && isspace(*command)) {
            *command = '\0';
            command++;
        }

        // Save position
        if (*command != '\0')
            tokens[i] = command;

        // Skip all non-whitespace characters
        while (*command != '\0' && !isspace(*command))
            command++;
    }
}

// Replaces the first '\n' found in [command] with '\0'
// Returns true if '\n' has been found and false otherwise
static bool stripNewline(Command command) {
    char *newlinePtr = strchr(command, '\n');
    if (newlinePtr != NULL) {
        *newlinePtr = '\0';
        return true;
    } else {
        return false;
    }
}

Status execute(Entity world, Command command) {
    if (*command == '#')
        return NO_STATUS;

    // Important - initializes tokens as an array of NULLs
    char **tokens = calloc(MAX_NUMBER_OF_ARGUMENTS + 1, sizeof(char *));

    bool hadNewLine = stripNewline(command);
    parse(command, tokens);

    Status status = ERROR;
    if (tokens[0] == NULL) {
        // Command consisted only of whitespaces - ignore
        status = NO_STATUS;

    } else if (!hadNewLine || tokens[MAX_NUMBER_OF_ARGUMENTS] != NULL) {
        status = ERROR;

    } else {
        // The first token is what to execute, the rest are a lineage
        Lineage lineage = tokens + 1;
        int lengthOfLineage = lineageLength(lineage);

        if (!validLineage(lineage)) {
            status = ERROR;

        } else if (strcmp(tokens[0], "ADD") == 0) {
            if (lineage[0] == NULL)
                status = ERROR;
            else
                status = add(world, lineage);

        } else if (strcmp(tokens[0], "DEL") == 0) {
            status = delete(world, lineage);

        } else if (strcmp(tokens[0], "PRINT") == 0) {
            if (lengthOfLineage > 2)
                status = ERROR;
            else
                status = print(world, lineage);

        } else if (strcmp(tokens[0], "CHECK") == 0) {
            if (lineage[0] == NULL || strcmp(lastInLineage(lineage), "*") == 0)
                status = ERROR;
            else
                status = check(world, lineage);

        }
    }

    free(tokens);
    return status;
}
