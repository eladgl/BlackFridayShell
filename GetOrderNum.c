#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#define PATH "/home/braude/Black_Friday/"
#define BUFF 128
#define ORDER "_Order"

int main(int argc, char* argv[]){
    int fileCount=0;
	char *path;
	DIR * dirp;
	struct dirent * entry;
    if(argc != 2){
        printf("This command gets only one parameter - company's name\n");
        exit(-1);
    }
    if((path = (char*)malloc(sizeof(char) * BUFF)) == NULL){
        printf("Allocation failure\n");
        exit(-1);
    } 
    strcpy(path, PATH);
    strcat(path, argv[1]);
    strcat(path, ORDER);

	dirp = opendir(path);
	if(dirp == NULL)
	{	
		free(path);
		printf("Company Not Found!\n");
		exit(-1);
	}

	while ((entry = readdir(dirp)) != NULL) {
		if (entry->d_type == DT_UNKNOWN) { //this linux image doenst get anything but this type for files.
		     fileCount++;
		}
	}
	closedir(dirp);
	printf("%s ---- %d orders\n",argv[1], fileCount);
    free(path);
    return 0;
}
