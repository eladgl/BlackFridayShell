#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

//dont forget to add the sentence Have Fun at the bottom
#define ALIGN "     "
#define BUFF 128
//dont forget to switch to /home/braude/
#define PATH "/home/braude/Black_Friday/"
#define NIS "NIS\n"
#define CAMP "camp_partics.txt"
#define CSV "_csv"

int write_to_file(int fd, char* str, int length);

void Error(char * msg) {printf("%s - Exiting Program...\n", msg); exit(-1);}

int main(int argc, char* argv[]){
    /**item is the list of items that will be inserted at the file
     * title is the allocated name for the file's name and will be printed at the top file
     * word is a pointer for a word used in strtok
    */
    char *item[BUFF], *title, *temp, *word, *file_name;
    int i,j, longest = 0, fd, item_count=0, free_i, choice, fd_csv;
    
    if(argc != 3)
        Error("You need to insert two parameters, company's name and discount percentage");
    if((title = (char*)malloc(sizeof(char) * BUFF)) == NULL)
        Error("Allocation failed");
    if((temp = (char*)malloc(sizeof(char) * BUFF)) == NULL){
        free(title);
        Error("Allocation failed");
    }
    if((file_name = (char*)malloc(sizeof(char) * (strlen(argv[1]) + sizeof(PATH)+5))) == NULL){ //+5 is for the .txt characters and end of string escape character
        free(title);
        free(temp);
        Error("Allocation failed");
    }
    strcpy(file_name, PATH);        //now argv[1].txt it will be created inside Black_Friday directory
    strcat(file_name, argv[1]);     //append the name
    strcat(file_name, "_Order");
    if(mkdir(file_name, 00777 ) == -1)
    {
        perror("mkdir: argv[1]_Order");
        free(title);
        free(file_name);
        free(temp);
        exit(-1);
    }

    //create csv folder names argv[1]_csv where there will be files with the item's name and price in csv format
    strcpy(file_name, PATH);
    strcat(file_name, argv[1]);
    strcat(file_name, CSV);
    if(mkdir(file_name, 00777) == -1){
        perror("mkdir: csv ");
        free(title);
        free(file_name);
        free(temp);
        exit(-1);
    }
    strcat(file_name, "/");
    strcat(file_name, argv[1]); //path = "PATH/argv[1]_csv/argv[1]_csv.txt"
    strcat(file_name, CSV);
    strcat(file_name, ".txt");
    if((fd_csv = open(file_name, O_RDONLY | O_WRONLY | O_CREAT, 0664)) == -1){
        perror("open: ");
        free(title);
        free(file_name);
        free(temp);
        exit(-1);
    }

    //add file_name to camp_partics.txt
    strcpy(file_name, PATH);
    strcat(file_name, CAMP);
    //strcat(file_name, argv[1]);

    if((fd = open(file_name, O_WRONLY | O_APPEND, 0664)) == -1){;
        perror("write: ");
        close(fd_csv);
        free(title);
        free(temp);
        exit(-1);
    } 
    if(write_to_file(fd, argv[1], strlen(argv[1]))==-1){
        close(fd);
        close(fd_csv);
        free(title);
        free(temp);
        exit(-1);
    }
    if(write_to_file(fd, "\n", strlen("\n"))==-1){
        close(fd);
        close(fd_csv);
        free(title);
        free(temp);
        exit(-1);
    }

    close(fd);
    strcpy(file_name, PATH);        //now argv[1].txt it will be created inside Black_Friday directory
    strcat(file_name, argv[1]);
    strcat(file_name, ".txt");      //request it to be a .txt file
    strcpy(title,ALIGN);            //results in:             argv[1] Sale
    strcat(title, argv[1]);         //                       argv[2] Sale
    strcat(title, " Sale\n");       //
    strcat(title, ALIGN);
    strcat(title, argv[2]);
    strcat(title, "% off\n");     // \\is an escape charater so % will be printed
    if((fd = open(file_name, O_WRONLY|O_CREAT ,0664)) == -1){
        free(title);
        free(temp);
        close(fd_csv);
        Error("Opening file failed");
    }

    if(write_to_file(fd, title, strlen(title)) == -1){
        perror("Could not write bytes to file stream");
            free(title);
            free(temp);
            close(fd);
            close(fd_csv);
            Error("File writing error");
    }
    
    free(title); //no longer need title - free it
    //begin insert all other lines of data to file
    fgets(temp,BUFF,stdin);             //get next line from user
    temp[strlen(temp)-1] = '\0';
    while(strcmp(temp, "Stop") != 0){
        if((item[item_count] = (char*)malloc(sizeof(char)*strlen(temp))) == NULL){
            //free all item[0..item_count-1]
            for(free_i =0; free_i < item_count; free_i++)
                free(item[free_i]);
            free(temp);
            close(fd);      //remember we already have freed *file_name
            close(fd_csv);
            Error("Item allocation was failed");
        }
        strcpy(item[item_count], temp);
        strcat(item[item_count], NIS);  //concatenate to each price at the end the word NIS
        if(longest < strlen(temp))      //calculate how many dots we need to print between product's name
            longest = strlen(temp);     //to its price in respct to max line's length
        item_count++;                   //save how many items were entered for used at writing
        fgets(temp,BUFF,stdin);         //get next line of items
        temp[strlen(temp)-1] = '\0';    //insert end of string to delete '\n'
    }
    //begin writing data
    for(i=0; i < item_count; i++){
        word = strrchr(item[i], ' '); //get last occurence of word to know which is the item's name and which is the price
        if(write_to_file(fd, item[i], strlen(item[i]) - strlen(word)) == -1){
            for(free_i =0; free_i < item_count; free_i++)
                free(item[free_i]);
            free(temp);
            close(fd_csv);
            close(fd);      //remember we already have freed *file_name
            Error("Item allocation was failed");          
        }
        //remember to write the same items to fd_csv
        if(write_to_file(fd_csv, item[i], strlen(item[i]) - strlen(word)) == -1){
            for(free_i =0; free_i < item_count; free_i++)
                free(item[free_i]);
            free(temp);
            close(fd_csv);
            close(fd);      //remember we already have freed *file_name
            Error("Item allocation was failed");          
        }
        //now write dots
        for(j=0; j < longest - strlen(item[i]) + strlen(word) ; j++){
            if(write_to_file(fd, ".", 1) == -1){
                for(free_i =0; free_i < item_count; free_i++)
                    free(item[free_i]);
                free(temp);
                close(fd);      //remember we already have freed *file_name
                Error("Item allocation was faile");    
            }
        }
        //now write price
        if(write_to_file(fd, word, strlen(word))==-1){
            for(free_i =0; free_i < item_count; free_i++){
                free(item[free_i]);
            }
            free(temp);
            close(fd_csv);
            close(fd);      //remember we already have freed *file_name
            Error("Item allocation was faile");    
        }
        //remember to write price to csv
        //before write '|'
        if(write_to_file(fd_csv, "|", 1) == -1){
            for(free_i =0; free_i < item_count; free_i++){
            free(item[free_i]);
            }
            free(temp);
            close(fd_csv);
            close(fd);      //remember we already have freed *file_name
            Error("Item allocation was faile");    
        }
        if(write_to_file(fd_csv, word, strlen(word)-4)==-1){ //the -4 is to delete NIS
            for(free_i =0; free_i < item_count; free_i++){
                free(item[free_i]);
            }
            free(temp);
            close(fd_csv);
            close(fd);      //remember we already have freed *file_name
            Error("Item allocation was faile");    
        }
        if(write_to_file(fd_csv, "\n", 1) == -1){
            for(free_i =0; free_i < item_count; free_i++)
                free(item[free_i]);
            free(temp);
            close(fd_csv);
            close(fd);      //remember we already have freed *file_name
            Error("Item allocation was faile");    
        }
        
    }
    //print message to choose
    printf("What is your choice?\n1.Enjoyed\n2.Shop Now and have fun\n3.Have Fun\n4.Other\n");
    scanf("%d", &choice);
    if(choice == 4){
        printf("Insert your word:\n");
        fflush(stdin);
        fflush(stdout);
        fgets(temp, BUFF, stdin);
        if(strcmp(temp, "\n") == 0)
            fgets(temp, BUFF, stdin);
        printf("temp = %s", temp);
        temp[strlen(temp)-1] = '\0';
    }
    else{
        if(choice == 1)
            strcpy(temp, "     Enjoyed");
        else if(choice == 2)
            strcpy(temp, "     Shop Now and have fun");
        else if(choice == 3)
            strcpy(temp, "     Have Fun");
    }
    if(write_to_file(fd, temp, strlen(temp))==-1){
            for(free_i =0; free_i < item_count; free_i++)
                free(item[free_i]);
            free(temp);
            close(fd);      //remember we already have freed *file_name
            Error("Item allocation was faile");    
        }
    printf("Successfully created\n");
    close(fd);
    close(fd_csv);
    free(temp);
    for(i = 0; i< item_count; i++)
        free(item[i]);
    exit(0);
}

//-1 is a fail write
//0 is a successfull write
int write_to_file(int fd, char* str, int length){
    int written = 0, wbytes;
    while(written < length){
        if((wbytes = write(fd,str + written, length - written)) < 0) //write one after the other
            return -1;
        written += wbytes;
    }
    return 0;
}
