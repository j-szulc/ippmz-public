
#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdbool.h>

#include "status.h"

typedef struct Node *BSTree;

// Type for storing the names of the entities
typedef char *Name;

typedef struct EntityData *Entity;

struct EntityData {
    Entity parentEntity;
    Name name;
    BSTree childrenEntities;
};

// Usage: createEntity(parent, name);
// Create entity with name [name] and [parent] as a parent
Entity createEntity(Entity, Name);

// Usage: freeAllChildren(entity);
// Frees all [entity] descendants from the memory and
// removes all values from [entity]->childrenEntities
void freeAllDescendants(Entity);

// Usage: freeEntity(entity);
// Frees [entity] along with all of its descendants from the memory
void freeEntity(Entity);

// Usage: disownEntity(entity);
// Detaches [entity] from its parent
// More precisely,
// removes appropriate [entity] from its parent childrenEntities BSTree
void disownEntity(Entity);

// A NULL-terminated array of names,
// where each two consequent non-NULL names belong to a parent and its child
// Example: {"name_of_a_forest", "name_of_a_tree_in_that_forest", NULL}
typedef Name *Lineage;

// Usage validLineage(lineage);
// Checks if every name in the [lineage]
// consists only of valid extended ASCII characters
bool validLineage(Lineage);

// Usage: lineageLength(lineage);
// Returns the number of non-NULL names in the [lineage]
int lineageLength(Lineage);

// Usage: lastInLineage(lineage);
// Returns the last non-NULL name in a lineage or NULL if it's empty
Name lastInLineage(Lineage);

#endif //ENTITIES_H
