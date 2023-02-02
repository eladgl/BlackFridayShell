#include "Black.h"

int execute_bin(char * cmd, char* path){
    int id, i;
    char *flags[5], *ptr, *command;
    int argc = 0;
    if((command = (char*)malloc(sizeof(char) * BUFF))==NULL){
        printf("Allocation failure\n");
        return -1;        
    }
    cmd[strlen(cmd)-1] = '\0';
    ptr = strtok(cmd, " ");
    while(ptr != NULL){
        if((flags[argc++] = strIncrease(ptr)) == NULL){
            for(i=0; i<argc; i++)
                free(flags[i]);
            free(command);
            printf("Allocation failure\n");
            return -1;
        }
        ptr = strtok(NULL, " ");
    } //remember to free flags[i]
    flags[argc] = NULL;
    
    if((id = fork()) == -1){
        perror("Failed on creating new process\n");
        for(i=0; i<argc; i++)
            free(flags[i]);
        free(command);
    }
    else if(id == 0){ //this is child process
        strcpy(command, path); //build command path
        strcat(command, flags[0]);
        execv(command, flags);
        
        if(errno == ENOENT)
            printf("Not Supported\n");
        else
            perror("execv: ");
        for(i=0; i<argc; i++)
            free(flags[i]);
        free(command);
        return -1;
    }
    else
        wait();
    for(i=0; i<argc; i++)
        free(flags[i]);
    free(command);
    return 1;
}

char *strIncrease(char *str){
    char *other = (char*)malloc(sizeof(char) *(strlen(str)+1));
    if(other != NULL)
        strcpy(other, str);
    return other;
}