
#ifndef BST_H
#define BST_H

// An interface for using the Binary Search Trees (BSTrees)

#include "status.h"
#include "entities.h"

struct Node;
typedef struct Node *BSTree;

typedef struct Node {
    Entity value;
    BSTree left, right;
} Node;

// Usage: insert(bsTreePtr, value);
// Inserts [value] into the BSTree pointed by [bsTreePtr]
// If a node with value [value] already exists, leaves the BSTree unchanged
Status insert(BSTree *, Entity);

// Usage: forEachValue(bsTree, function);
// Executes [function] with value as an argument, for each value in the bsTree
// in lexicographical order
void forEachValue(BSTree, void (*)(Entity));

// Usage: removeValue(bsTreePtr, value);
// Removes the node with the [value] from the BSTree pointed by [bsTreePtr]
// Leaves the BSTree unchanged if the [value] doesn't occur
void removeValue(BSTree *, Entity);

// Usage: removeAllValues(bsTree);
// Removes all nodes from the [bsTree]
void removeAllValues(BSTree);

// Usage: findValueByName(bsTree, name);
// Returns the value with name equal to [name] if it exists in [bsTree]
// NULL is returned otherwise
Entity findValueByName(BSTree, Name);

#endif //BST_H
