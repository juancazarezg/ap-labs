  
#include <stdio.h>
#include "logger.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <signal.h>
#include <syslog.h>


#define RED 1
#define YELLOW 3
#define MAGENTA 5
#define WHITE 7
#define GREEN 2

int systemlog = 0;

int initLogger(char *logType) {

    if (strcmp(logType, "syslog") == 0)
    {
        systemlog = 1;
        printf("Initializing Logger on: syslog\n");
    }
    else if (strcmp(logType, "stdout") == 0 || strcmp(logType, "") == 0)
    {
        printf("Initializing Logger on: stdout\n");
    }
    else
    {
        printf("Not a proper value");
        return 1;
    }

    return 0;
}

void txtcolor(int fg)
{
    char color[13];
    sprintf(color, "%c[%dm", 0x1B, fg + 30);
    printf("%s", color);
}

int infof(const char *format, ...) {
    txtcolor(GREEN);
    va_list arg;
    va_start(arg, format);

    if (systemlog == 1)
    {
        openlog("mylogger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }
    else
    {
        vprintf(format, arg);
    }

    va_end(arg);
    txtcolor(WHITE);
    return 0;
}

int warnf(const char *format, ...) {
    txtcolor(YELLOW);
    va_list arg;
    va_start(arg, format);

    if (systemlog == 1)
    {
        openlog("mylogger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_WARNING, format, arg);
        closelog();
    }
    else
    {
        vprintf(format, arg);
    }

    va_end(arg);
    txtcolor(WHITE);
    return 0;
}

int errorf(const char *format, ...) {
    txtcolor(RED);
    va_list arg;
    va_start(arg, format);

    if (systemlog == 1)
    {
        openlog("mylogger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_ERR, format, arg);
        closelog();
    }
    else
    {
        vprintf(format, arg);
    }

    va_end(arg);
    txtcolor(WHITE);
    return 0;
}
int panicf(const char *format, ...) {
    txtcolor(MAGENTA);
    va_list arg;
    va_start(arg, format);

    if (systemlog == 1)
    {
        openlog("mylogger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(LOG_EMERG, format, arg);
        closelog();
    }
    else
    {
        vprintf(format, arg);
    }

    va_end(arg);
    txtcolor(WHITE);
    raise(SIGABRT);
    return 0;
}