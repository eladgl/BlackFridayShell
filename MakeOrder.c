#include "Order.h"

int main(int argc, char* argv[]){
    /*path will be used in getting to different locations in tree directory
    ** item holds the item's name in the csv at the argv[1]_csv.txt
    ** pid is used at creating new processes
    ** each fd_flyer will hold another file respected to its name*/

    char *path,*what_to_write, *order, *item, *file_text, *line,*order_line, *temp, *csv_item, *quantity, total_str[BUFF];
    int fd_flyer_order,fd, pid;
    float total = 0, discount;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if(argc != 3){
        printf("Please enter only company name and the name of who orders");
        exit(-1);
    }
    if((path = (char *)malloc(sizeof(char) * BUFF)) == NULL){
        printf("allocation Error");
        exit(-1);
    }
    strcpy(path, PATH);
    strcat(path,argv[1]);
    strcat(path,TXT);
    //check if given company name is in list
    if((fd = open(path,O_RDONLY)) == -1){ //if company flyer does not exist print and exit
        free(path);
        printf("Company Not Found!\n");
        exit(-1);
    }
    //print requested flyer
    
    if((temp = (char *)malloc(sizeof(char) *10* BUFF)) == NULL){
        ERROR("Allocation Failure", path, NULL, NULL, NULL, NULL, NULL);
    }
    if((order = (char *)malloc(sizeof(char) * BUFF)) == NULL){
        ERROR("Allocation Failure",  temp, path, NULL, NULL, NULL, NULL);
    }
    if((what_to_write = (char *)malloc(sizeof(char) * 20*BUFF)) == NULL){
        ERROR("Allocation Failure", temp, order, path, NULL, NULL, NULL);
    }
    
    strcpy(path, GETFLYER);//get the GetFlyer executable path
    if((pid = fork()) == -1){ //begin executation 
        perror("fork: ");
        ERROR("", temp, order, path, NULL, NULL, NULL);
    }
    else if(pid == 0){
        execl(GETFLYER, FLYER, argv[1], NULL); //print the flyer
        perror("execv: ");
        ERROR("", temp, order, path, NULL, NULL, NULL);
    }
    else
        wait();
    //get path to point at argv[1].txt
    
    strcpy(path, PATH); //we do this to get the discount out of the argv[1].txt flyer
    strcat(path, argv[1]);
    strcat(path, TXT); 
    if((file_text = read_file(path)) == NULL){ //has the bug.txt output
        ERROR("Allocation failure\n", temp, order, path, NULL, NULL, NULL);
    }
    //to get discount we take the first line throw it and the second line
    line = strtok(file_text, "\n");
    line = strtok(NULL, "\n");
    discount = getDiscount(line);
    free(file_text);
    //get path to point at csv file location
    
    strcpy(path, PATH);
    strcat(path, argv[1]);
    strcat(path, CSV);
    strcat(path, "/");
    strcat(path, argv[1]);
    strcat(path,CSV);
    strcat(path, TXT);           //now we point to the correct file
    file_text = read_file(path); //has the bug_csv.txt output
    strcpy(temp, file_text);     //maintain a copy of the csv so we can loop the products
    //create flyer for argv[1]_Order.txt
    strcpy(path,PATH);
    strcat(path, argv[1]); 
    strcat(path, ORDER);
    strcat(path, "/");
    strcat(path,argv[2]);
    strcat(path,TXT);
    if((fd_flyer_order = open(path, O_WRONLY | O_CREAT, 0664)) == -1){
        ERROR("Could not create order - please try again\n", temp, order, path, file_text, NULL, NULL);
    }
    strcpy(what_to_write, argv[1]);
    strcat(what_to_write, " Order\n\n");
    printf("Insert your order (STOP to finish):\n");
    fgets(order, BUFF, stdin);
    order[strlen(order) - 1] = '\0';
    while (strcmp(order, "STOP") != 0)
    {
        order_line = strtok(file_text, "\n"); //order_line is the item from csv file
        while(order_line != NULL){ //iterate csv file to see if entered item in order exists in item in csv
            item = getItem(order, 32);
            //printf("item got from getItem: %s\n", item);
            if(item == NULL){ //this means allocation for item failed
                close(fd_flyer_order);
                ERROR("Error getting order\n", temp, order, path, file_text, NULL, NULL);
            }
            csv_item = get_csv_item(order_line);
            //printf("csv_item got from get_csv_item: %s\n", csv_item);
            if(csv_item == NULL){
                close(fd_flyer_order);
                ERROR("Error getting order\n", temp, order, path, file_text, item, NULL);
            }  
            if(strcmp(item,csv_item) == 0){
                if((quantity = getQty(order, 32)) == NULL){ //if it is the same item proceed to take its quantity
                    close(fd_flyer_order);
                    ERROR("Error getting quantity\n", temp, order, path, file_text, item, csv_item);
                }
                //printf("quntity got from getQty: %s\n", quantity);

                total += convertStrToNum(quantity) * get_csv_price(order_line, discount);
                //if we got up to here this means we need to concatenate the information
                //write to order file
                strcat(what_to_write, item);
                free(item);
                free(csv_item);
                strcat(what_to_write, " - ");
                strcat(what_to_write, quantity);
                free(quantity);
                strcat(what_to_write, "\n\n");
                break;                          //we found the item in csv list we can go to next order
            }
            if(order_line == NULL){
                printf("Item is not in company's black friday item's list\n");
            }
            if(item != NULL){free(item);}                         //free this memory so next loop we can allocate new memory for item
            if(csv_item != NULL){free(csv_item);}                 //free this memory so next loop we can allocate new memory for csv_item
            order_line = strtok(NULL, "\n"); //go to next line
        }
        fgets(order, BUFF, stdin); //get next order
        order[strlen(order) - 1] = '\0';
        strcpy(file_text, temp);
    }
    //no longer need file_text
    free(file_text);
    sprintf(total_str, "%.2f", total);
    printf("Total Price: %s NIS (Confirm to approve/else cancle)\n", total_str);
    scanf("%s", order);
    while(strcmp(order, "cancel") != 0){
        if(strcmp(order, "Confirm") == 0){
            //write price to file /"\e[1mMd.Mehedi hasan\e[m"
            strcat(what_to_write, "Total Price: ");
            strcat(what_to_write, total_str);
            strcat(what_to_write, " NIS");
            strcat(what_to_write, "\n\n");
            //get time
            sprintf(temp, "%d", tm.tm_mday);
            strcat(what_to_write, temp);
            strcat(what_to_write, "/");
            sprintf(temp, "%d", tm.tm_mon + 1);
            strcat(what_to_write, temp);
            strcat(what_to_write, "/");
            sprintf(temp, "%d", tm.tm_year + 1900);
            strcat(what_to_write, temp);
            if((write_to_file(fd_flyer_order, what_to_write, strlen(what_to_write))) == -1){
                close(fd_flyer_order);
                Error("Error writing to file\n", temp, order, path, item, what_to_write, NULL);
            }
            //change mode for file
            
            printf("Order created!\n");
            if((pid = fork()) == -1){
                perror("fork :");
                Error("", temp, order, path, item, what_to_write, NULL);
            }
            else if(pid == 0){ //child process
                execl("/bin/chmod", "chmod","555" ,path, NULL);
                perror("execl: ");
                Error("", temp, order, path, item, what_to_write, NULL);
            }
            else{
                wait();
            }
            
            break; //option was selected
        }
        else{
            printf("Please choose Confirm or cancel\n");
            scanf("%s", order);
        }
    }
    free(order);
    free(path);
    free(what_to_write);
    free(temp);
    free(item);
    close(fd_flyer_order);
    exit(0);
}
