#define _XOPEN_SOURCE 500
#include <stdio.h>
#include "logger.h"
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ftw.h>
#include <unistd.h>


#define BUF_LEN (10 * (sizeof(struct inotify_event) + 100 + 1))

int inotifyFd, wd;
char buf[BUF_LEN] __attribute__ ((aligned(8)));
ssize_t numRead;
char *p;
struct inotify_event *event;


static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{

    if(ftwbuf->level < 3){
        wd = inotify_add_watch(inotifyFd, fpath, IN_CREATE | IN_DELETE | IN_MOVED_FROM);
        if (wd == -1)
            errorf("inotify_add_watch");

    }
}

static int *displayInotifyEvent(struct inotify_event *i)
{
    infof("- [");
    if (i->mask & IN_ISDIR){
        infof("Directory - ");
    }else{
        infof("File - ");
    }
    if (i->mask & IN_MOVED_FROM) infof("Rename");
    if (i->mask & IN_CREATE) infof("Create");
    if (i->mask & IN_DELETE) infof("Removal");
    if (i->len > 0) infof("] - %s\n", i->name);

    return 0;
}


int
main(int argc, char *argv[])
{
    inotifyFd = inotify_init();                 /* Create inotify instance */
    if (inotifyFd == -1)
        errorf("inotify_init");

    int flags = 0;


    if (argc > 2 && strchr(argv[2], 'd') != NULL)
        flags |= FTW_DEPTH;
    if (argc > 2 && strchr(argv[2], 'p') != NULL)
        flags |= FTW_PHYS;

    if(argv[1]== NULL){
        errorf("Insert a path as an argument\n");
        exit(EXIT_FAILURE);
    }

    infof("Starting File/Directory Monitor on %s\n", argv[1]);
    infof("-----------------------------------------------------\n");
    
    if (nftw(argv[1], display_info, 20, flags) == -1)
    {
        errorf("nftw");
    }

    for (;;) {                                  /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            errorf("read() from inotify fd returned 0!");

        if (numRead == -1)
            errorf("read");

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }


    exit(EXIT_SUCCESS);
}