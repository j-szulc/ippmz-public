// If not defined, results in a warning with getline
#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

void output(Status status) {
    if (status == OK)
        printf("OK\n");
    else if (status == YES)
        printf("YES\n");
    else if (status == NO)
        printf("NO\n");
    else if (status == ERROR)
        fwrite("ERROR\n", 1, sizeof("ERROR"), stderr);
    else if (status == MALLOC_ERROR)
        exit(1);
}

int main(int argc, char **argv) {
    // A top entity
    // Every other entity is a descendant of it
    Entity world = createEntity(NULL, NULL);

    FILE *input = stdin;
    if (argc > 1) {
        input = fopen(argv[1], "r");
    }

    //input = fopen("../testinput", "r");
    char *command = NULL;
    size_t len = 0;
    while (getline(&command, &len, input) != -1) {
        output(execute(world, command));
    }

    // EOF received
    free(command);
    fclose(input);
    freeEntity(world);
    return 0;
}