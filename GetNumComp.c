#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define PATH "/home/braude/Black_Friday/camp_partics.txt"
int main(int argc, char *argv[]){
    int fd, rbytes, length, count = 0;
    char* text, *line;
    if(argc > 1){
        printf("This command should not take any parameters");
        exit(-1);
    }
    
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
        count++;
        line = strtok(NULL, "\n");
    }
    free(text);
    printf("%d companies take part in the Campaign\n", count);
    return 0;
}