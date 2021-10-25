#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPORT_FILE "report.txt"
#define HASH_SIZE 10000
#define LINES 5000

void analizeLog(char *logFile, char *report);
void insert(char *key, char *val);
int readFile(char *file);
unsigned long hash(char *key);

struct Log {
    char *key, val[HASH_SIZE];
};

struct Log *hashArray[HASH_SIZE];
char *arrLines[HASH_SIZE];

unsigned long hash(char *key) {
    unsigned value;
    for(value = 0; *key != '\0'; key++){
        value = *key + 31 *value;
        return  value % HASH_SIZE;
    }
}

void insert(char *key, char *val) {
    struct Log *log = (struct Log*) malloc (sizeof(struct Log));
    int idx = hash(key);
    
    while(hashArray[idx] != NULL){
        if(strcmp(hashArray[idx]->key, key) == 0){
            strcat(hashArray[idx]->val, "\t");
            strcat(hashArray[idx]->val, val);
            
            return;
        }
        ++idx;
        idx %= HASH_SIZE;
    }

    log->key = (char*) malloc(100);
    strcpy(log->key, key);
    strcat(log->val, "\t");
    strcat(log->val, val);

    hashArray[idx] = log;

}

int readFile(char *file){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long totaLines = 0;

    fp = fopen(file, "r");
    if (fp == NULL){
        printf("No info found, try with a different file. \n");
        exit(EXIT_FAILURE);
    }
        
    int current = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        arrLines[current] = malloc(read + 1);
        if (strlen(line) == 0 || strcasecmp(line, "\n") == 0) continue;
        strcpy(arrLines[current], line);
        current++;
        totaLines++;
    }

    fclose(fp);

    
    return totaLines;
}


void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.
    int num_of_lines = readFile(logFile);
    int i;

    for(i = 0; i < num_of_lines; i++){
        int size = strlen(arrLines[i]);
        char key[LINES] = "\0";
        char time[LINES], message[LINES], second_message[LINES];
        int points, countKey, countMessage, j;
        points = countKey = countMessage = 0;

        for(j = 0; j < size; j++){
            if(points == 0){
                if(!(arrLines[i][j - 1] == ':' && arrLines[i][j] == ' ')){
                    key[countKey] = arrLines[i][j];
                    countKey++;
                }else{
                    points++;
                }
            }else if (j >= 13){
                second_message[countMessage] = arrLines[i][j];
                countMessage++;
            }else{
                if(!(arrLines[i][j - 1] == ']' && arrLines[i][j] == ' ')){
                    time[j] = arrLines[i][j];
                }
            }
        }

        if(points == 0){
            strcpy(second_message, "");
            strcpy(second_message, key);
            strcpy(key, "General");
        }

        strcat(message, time);
        strcat(message, " ");
        strcat(message, second_message);

        insert(key, message);

        memset(message, 0, LINES);
        memset(second_message, 0, LINES);
        memset(key, 0, LINES);
        memset(time, 0, LINES);
    }

    int reportFile = open(report, O_CREAT | O_WRONLY, 0600);
    int idx;

    if(reportFile < 0){
        printf("File couldn't be generated, try another filename.");
        return;
    }

    for(idx = 0; idx < HASH_SIZE; idx++) {
        if(hashArray[idx] != NULL){
            char line[100000];
            snprintf(line, 100000, "%s \n%s\n", hashArray[idx]->key, hashArray[idx]->val);
            write(reportFile, line, strlen(line));
        }
    }
    close(reportFile);


    printf("Report is generated at: [%s]\n", report);
}

int main(int argc, char **argv) {

    if (argc < 2) {
	    printf("Usage:./dmesg-analizer logfile.txt\n");
	    return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

// Made by: Joel Fernando Santillan Santana - A01634748

// References: 
// readFile function taken from https://linux.die.net/man/3/getline