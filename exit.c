#include <stdio.h>
#include <fcntl.h>

int main(){
    char *rm_execute[] = {"rm","-r","-f","Black_Friday\0" , NULL};
    int id;
    id = fork();
    if(id > 0)
        wait();
    else if(id == -1)
        perror("failed to create process");
    else if(id == 0)
        execvp(rm_execute[0],rm_execute);
    return 0;
}
