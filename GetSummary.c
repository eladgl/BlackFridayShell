#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define PATH "/home/braude/Black_Friday/camp_partics.txt"
#define GET_ORDER_NUM "/home/braude/GetOrderNum"
#define GET_NUM_COMP "/home/braude/GetNumComp"
#define ORDER "GetOrderNum"

int main(int argc, char *argv[]){
    int fd, rbytes, length, pid;
    char* text, *line;
    if(argc > 1){
        printf("This command should not take any parameters");
        exit(-1);
    }
    //create nre process
    if((pid = fork()) == -1){
        perror("fork :");
        exit(-1);
    }
    else if(pid == 0){
        execl(GET_NUM_COMP, "GetNumComp", NULL);
        perror("execl: ");
        exit(-1);
    }
    else{
        wait();
    }
    //get the camp_partics.txt text - know which companies exists - they have orders
    if((fd = open(PATH,O_RDONLY)) == -1){ //if company flyer does not exist print and exit
        perror("open: ");
        exit(-1);
    }
    if((length = lseek(fd, 0, SEEK_END)) == -1){
        perror("seek to end\n");
        close(fd);
        exit(-1);
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("seek to begining");
		close(fd);
		exit(-1);
	}
    if((text = (char*)malloc(sizeof(char) * length)) == NULL){
		close(fd);
        printf("Allocation for saving file's data length failed");
        exit(-1);
    }
    if((rbytes = read(fd, text, length)) == -1){
        perror("read failed");
        free(text);
		close(fd);
		exit(-1);
    }
    text[rbytes] = '\0'; //delete all binary data or line breaks that might have been read
                         //because read also read bin data

    //begin counting distinct lines in camp_partics.txt
    line = strtok(text, "\n");
    while(line != NULL){
        if((pid = fork()) == -1){
            perror("fork: ");
            exit(-1);
        }
        else if(pid == 0){ //child process
            execl(GET_ORDER_NUM, ORDER, line ,NULL); //print each company and how many orders it has
            perror("execl : ");
            exit(-1);
        }
        else{
            wait(NULL);
        }
        line = strtok(NULL, "\n");
    }
    free(text);
    return 0;
}