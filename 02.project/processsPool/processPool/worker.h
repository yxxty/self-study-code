#ifndef __WORKER_H__
#define __WORKER_H__
#include <myfunc.h>

enum {
    FREE,
    BUSY,
};

typedef struct WorkerData_s{
    pid_t pid;
    int status;
    int pipeFd;
} WorkerData_t;

int makeWorker(WorkerData_t *data, int num);
int handleEvent(int pipeFd);

#endif