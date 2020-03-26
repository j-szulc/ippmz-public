// If not defined, results in a warning with strdup
#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "entities.h"
#include "bst.h"

Entity createEntity(Entity parentEntity, Name name) {
    Entity entity = (Entity) malloc(sizeof(struct EntityData));

    if (entity != NULL) {
        // We allow entity to have a NULL name
        Name nameCopy = NULL;
        if (name != NULL) {
            // Important - hidden malloc
            nameCopy = (Name) strdup(name);
        }

        entity->name = nameCopy;
        entity->parentEntity = parentEntity;
        entity->childrenEntities = NULL;

        // We allow entity to have a NULL parent
        if (parentEntity != NULL) {
            Status status = insert(&(parentEntity->childrenEntities), entity);
            if (status == MALLOC_ERROR)
                return NULL;
        }
    }

    // returns NULL if there has been a malloc error
    return entity;
}

void freeAllDescendants(Entity entity) {
    if (entity != NULL) {
        forEachValue(entity->childrenEntities, &freeEntity);
        removeAllValues(entity->childrenEntities);
        entity->childrenEntities = NULL;
    }
}

void freeEntity(Entity entity) {
    if (entity != NULL) {
        freeAllDescendants(entity);
        free(entity->name);
        free(entity);
    }
}

void disownEntity(Entity entity) {
    if (entity != NULL && entity->parentEntity != NULL) {
        removeValue(&(entity->parentEntity->childrenEntities), entity);
    }
}

// Checks if all characters in [name] are valid extended ASCII characters
static bool validName(Name name) {
    if (name == NULL) {
        return true;
    } else if (*name == '\0') {
        return false;
    } else {
        bool valid = true;
        while (*name != '\0' && valid) {
            valid = valid && (*name > 32 || *name < 0) && !isspace(*name);
            name++;
        }
        return valid;
    }
}

bool validLineage(Lineage lineage) {
    if (*lineage == NULL)
        return true;
    else
        return validName(*lineage) && validLineage(lineage + 1);
}

int lineageLength(Lineage lineage) {
    if (*lineage != NULL)
        return lineageLength(lineage + 1) + 1;
    else
        return 0;
}

Name lastInLineage(Lineage lineage) {
    int lengthOfLineage = lineageLength(lineage);
    if (lengthOfLineage == 0)
        return NULL;
    else
        return lineage[lengthOfLineage - 1];
}


