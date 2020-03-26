#include <stdlib.h>
#include <string.h>

#include "bst.h"
#include "entities.h"

Status insert(BSTree *bsTreePtr, Entity value) {
    BSTree bsTree = *bsTreePtr;

    if (bsTree == NULL) {
        bsTree = (BSTree) malloc(sizeof(struct Node));
        if (bsTree == NULL)
            return MALLOC_ERROR;

        bsTree->value = value;
        bsTree->left = bsTree->right = NULL;
        *bsTreePtr = bsTree;

        return OK;
    } else {
        int compareResult = strcmp(bsTree->value->name, value->name);

        if (compareResult > 0)
            return insert(&(bsTree->left), value);
        else if (compareResult < 0)
            return insert(&(bsTree->right), value);
        else
            return OK;
    }
}

void forEachValue(BSTree bsTree, void (*function)(Entity)) {
    if (bsTree != NULL) {
        forEachValue(bsTree->left, function);
        function(bsTree->value);
        forEachValue(bsTree->right, function);
    }
}

// Removes the node with the minimal value in the BSTree pointed by [bsTreePtr]
// Returns this minimal value
static Entity removeMinValue(BSTree *bsTreePtr) {

    BSTree bsTree = *bsTreePtr;
    Entity minimum;

    if (bsTree->left == NULL) {
        minimum = bsTree->value;
        *bsTreePtr = bsTree->right;
        free(bsTree);
        return minimum;
    } else {
        return removeMinValue(&(bsTree->left));
    }
}

void removeValue(BSTree *bsTreePtr, Entity value) {
    BSTree bsTree = *bsTreePtr;

    if (bsTree != NULL) {
        int compareResult = strcmp(bsTree->value->name, value->name);

        if (compareResult > 0) {
            removeValue(&(bsTree->left), value);
        } else if (compareResult < 0) {
            removeValue(&(bsTree->right), value);
        } else {

            if (bsTree->right == NULL) {
                *bsTreePtr = bsTree->left;
                free(bsTree);
            } else {
                bsTree->value = removeMinValue(&(bsTree->right));
            }

        }
    }
}

void removeAllValues(BSTree bsTree) {
    if (bsTree != NULL) {
        removeAllValues(bsTree->left);
        removeAllValues(bsTree->right);
        free(bsTree);
    }
}

Entity findValueByName(BSTree bsTree, Name name) {
    if (bsTree == NULL)
        return NULL;

    int compareResult = strcmp(bsTree->value->name, name);
    if (compareResult > 0)
        return findValueByName(bsTree->left, name);
    else if (compareResult < 0)
        return findValueByName(bsTree->right, name);
    else
        return bsTree->value;
}

