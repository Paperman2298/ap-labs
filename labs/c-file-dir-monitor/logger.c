#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <stdbool.h>
#include "logger.h"

bool flag=false;

int initLogger(char *logType) { // Checks if the logType is valid
    printf("Initializing Logger on: %s\n", logType);

    if (strcmp(logType, "syslog") == 0){
        flag = true;
    }else if (strcmp(logType, "stdout") == 0 || strlen(logType) == 0){
        flag = false;
    }else{
        printf("LogType cannot be understood, try a different argument. \n");
    }
    return 0;
}

int infof(const char *format, ...) {
    va_list args; // Object type suitable for holding the information needed by the macros
    va_start(args, format);

    if(flag){
        vsyslog(LOG_NOTICE, format, args); // Normal, but significant, condition
        closelog();
    }else{
        vprintf(format, args);
    }

    printf("\n");

    va_end(args);

    return 0;
}

int warnf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    if(flag){
        vsyslog(LOG_WARNING, format, args); // Warning conditions
        closelog();
    }else{
        vprintf(format, args);
    }

    printf("\n");

    va_end(args);

    return 0;
}

int errorf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    if(flag){
        vsyslog(LOG_ERR, format, args); // Error conditions
        closelog();
    }else{
        vprintf(format, args);
    }

    printf("\n");

    va_end(args); 

    return 0;
}

int panicf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    if(flag){
        vsyslog(LOG_CRIT, format, args); // Critical conditions
        closelog();
    }else{
        vprintf(format, args);
    }

    printf("\n");

    va_end(args);

    return 0;
}

