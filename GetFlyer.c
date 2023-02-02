#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

//dont forget to switch to /home/braude/
#define PATH "/home/braude/Black_Friday/"
#define TXT ".txt"

void Error(char * msg) {printf("%s\n", msg); exit(-1);}

int main(int argc, char* argv[]){
    int fd, rbytes, length;
    char* text, *path;
    if(argc != 2)
        Error("GetFlyer Should only get one parameter");
    
    if((path = (char *)malloc(sizeof(char) * (strlen(argv[1]) + sizeof(PATH) + sizeof(TXT)))) == NULL)
        Error("path name allocation failure\n");
    strcpy(path, PATH);
    strcat(path,argv[1]);
    strcat(path,TXT);

    if((fd = open(path,O_RDONLY)) == -1){ //if company flyer does not exist print and exit
        free(path);
        Error("Company Not Found!\n");
    }
    if((length = lseek(fd, 0, SEEK_END)) == -1){
        perror("seek to end\n");
        free(path);
        close(fd);
        exit(-1);
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("seek to begining");
        free(path);
		close(fd);
		exit(-1);
	}
    if((text = (char*)malloc(sizeof(char) * length)) == NULL){
        free(path);
		close(fd);
        Error("Allocation for saving file's data length failed");
    }
    if((rbytes = read(fd, text, length)) == -1){
        perror("read failed");
        free(path);
		close(fd);
		exit(-1);
    }
    text[rbytes] = '\0'; //delete all binary data or line breaks that might have been read
                         //because read also read bin data
    printf("%s\n", text);
    close(fd);
    free(path);
    free(text);
    exit(0); //return 0 on success
}