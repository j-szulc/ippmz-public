
#ifndef CLI_H
#define CLI_H

// An interface to interact with entities with commands

#include "status.h"
#include "entities.h"
#include "bst.h"

// Maximum number of tokens needed for any command
// Example: command "ADD a b c " requires 4 tokens
#define MAX_NUMBER_OF_ARGUMENTS 4

// Type for storing the commands
typedef char *Command;

// Usage: execute(world, command);
// Executes [command] with [world] as the highest entity
// Example: execute(world, " ADD forest tree\n ");
Status execute(Entity, Command);

#endif //CLI_H
