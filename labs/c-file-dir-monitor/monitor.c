#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <stdbool.h>
#include "logger.h"

int main(int argc, char **argv){
    // Place your magic here

    if(argc < 2){
        printf("Missing arguments; \n");
        printf("Try using ./monitor /someDirectory");
        return 404;
    }

    char* current_path = argv[1];
    char arr[10000];
    int file = inotify_init(); // Returns a file descriptor associated with a new inotify event queue.

    if(file < 0){
        printf("Not able to initialize notify, check if file exists. \n");
        return 404;
    }

    int watchList = inotify_add_watch(file, current_path, IN_CREATE | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE);
    // Each item in the watch list specifies the pathname of a file or directory and the event

    if(watchList > 0){
        printf("Starting file: %s. \n", current_path);
    }else{
        printf("Couldn't add %s to watch list. \n", current_path);
    }

    int i;
    bool flag = true;
    while(flag){
        i = 0;
        int r = read(file, arr, 10000);

        if(r < 0){
            printf("Couldn't read file. \n");
            break;
        }

        while(i < r){
            struct inotify_event *event = (struct inotify_event*) &arr[i]; // Describes a filesystem event
            if(event -> len){
                if(event -> mask & IN_CREATE){ // Check for creation

                    // Check if its file or directory
                    if(event -> mask & IN_ISDIR){ // Subject of this event is a directory
                        infof("[Directory - Create]: %s\n", event -> name);
                    }else{
                        infof("[File - Create]: %s\n", event -> name);
                    }
                }else if(event -> mask & IN_MOVED_FROM){ // Check for movement from
                    if(event -> mask & IN_ISDIR){
                        infof("[Directory - Rename]: %s\n", event -> name);
                    }else{
                        infof("[File - Rename]: %s\n", event -> name);
                    }
                }else if(event -> mask & IN_MOVED_TO){ // Check for move to
                    if(!(event -> mask & IN_ISDIR)){
                        infof("-> %s\n", event -> name);
                    }
                }else if(event -> mask & IN_DELETE){ // Check for delete
                    if(event -> mask & IN_ISDIR){
                        infof("[Directory - Removal]: %s\n", event -> name);
                    }else{
                        infof("[File - Removal]: %s\n", event -> name);
                    }
                }

                i += event -> len;
            }
        }
    }

    inotify_rm_watch(file, watchList); // Remove an existing watch from an inotify instance
    close(file);

    return 0;
}
