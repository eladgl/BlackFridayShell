#include "Black.h"

int main(int argc, char* argv[]){
    int fd, id_del,succ; //exit flag tells if other commands were used before exit to free flags[j]
    char *path, *cmd;
    enum Command i;
    //allocating memory
    if((path = (char*)malloc(sizeof(char) * BUFF)) == NULL){
        exit(-1);
    }
    if((cmd = (char*)malloc(sizeof(char) * BUFF)) == NULL){
        free(path);
        exit(-1);
    }
    //create Black_Friday DIR 
    if((succ = mkdir("/home/braude/Black_Friday", 0766)) == -1){
        perror("Cannot create Black Friday dir\n"); //maybe directory already exists take care of it later
        free(path);
        free(cmd);
        exit(-1);
    }

    if((fd = open("/home/braude/Black_Friday/camp_partics.txt",  O_WRONLY | O_CREAT, 0660)) == -1){
        free(path);
        free(cmd);
        perror("open file error\n");
        exit(-1);
    }
    printf("\nAdvShell> ");
    fgets(cmd, BUFF, stdin);
    while(strcmp(cmd, "exit\n") != 0){
        //check if its a command of our shell
        for(i = 0; i < END_RUN; i++){
            if(strstr(cmd, command_str[i]) != NULL){ 
                execute_bin(cmd, BLACK_PATH); //execute black friday executables
                break;
            }
        }
        if(i == END_RUN){
            execute_bin(cmd, BIN_PATH); //execute system commands
        }
        strcpy(path, "");           //initialize path back to empty string
        printf("AdvShell> ");
        fflush(stdin);
        strcpy(cmd, "");            //initialize the command golder
        fgets(cmd, BUFF, stdin);    //read next command      
    }
    //delete Black Friday and print a GoodBye message
    if((id_del = fork() == -1))
        perror("Print create process failed\n");
    else if(id_del == 0){ //this is the child who initiates the directory deletion
        execl("/home/braude/exit", "exit", NULL);
        perror("Failed to delete Black_Friday directory.\n");
    }
    else{
        wait();
        printf("GoodBye...\n");
    }
    
    //now the parent process will close all file descriptors and end the run
    close(fd);
    free(path);
    free(cmd);
    exit(0);
}