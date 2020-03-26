
#ifndef STATUS_H
#define STATUS_H

// All possible CLI commands status codes
typedef enum {
    OK = 0, YES = 1, NO = 2, ERROR = 3, MALLOC_ERROR = 4, NO_STATUS = 5
} Status;

#endif //STATUS_H
