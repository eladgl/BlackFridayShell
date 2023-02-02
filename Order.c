#include "Order.h"

void Error(char* msg, char* mem1, char* mem2, char* mem3, char* mem4, char* mem5, char* mem6) {
	//int i;
    fprintf(stderr, "Error:%s- Exiting Program...\n", msg);
    //for(i = 0; i < strlen(mem); i++)
	if (mem1 != NULL) {
		if (mem2 != NULL) {
            if(mem3 != NULL){
                if(mem4 != NULL){
                    if(mem5 != NULL){
                        if(mem6 != NULL){
                            free(mem6);
                        }
                        free(mem5);
                    }

                    free(mem4);
                }
                free(mem3);
            }
			free(mem2);
		}
		free(mem1);
	}
	exit(-1);
}

float getDiscount(char* line){
    char *ch, *percent, discount_ptr[3];
    ch = trim(line);  //trim spaces          
    percent = strchr(line, 37); // get index of '%' 37 is its ascii
    strncpy(discount_ptr, ch, strlen(ch) - strlen(percent));
    return convertStrToNum(discount_ptr)/100; //this is the discount
}

char* get_csv_item(char* line){
    char* pipe_ptr = strchr(line, 124), *item; //ascii of |
    if((item = (char*)malloc(sizeof(char) * BUFF)) == NULL)
        return NULL;
    strncpy(item, pipe_ptr - (strlen(line) - strlen(pipe_ptr)), strlen(line) - strlen(pipe_ptr)); //returns the item
    item[strlen(line) - strlen(pipe_ptr)] = '\0';
    return item;
}

float get_csv_price(char* line, float discount){
    char* pipe_ptr = strchr(line, 124), *csv_price;
    float cost;
    if((csv_price = (char*)malloc(sizeof(char) * BUFF)) == NULL)
        return -1; //in the main will check if -1, it means allocation failed
    strncpy(csv_price, pipe_ptr +2, strlen(pipe_ptr)+2);
    csv_price[strlen(pipe_ptr)+2] = '\0';
    cost = atoi(csv_price);
    free(csv_price);
    return cost * discount;
}

char *ltrim(char *str){
    while (isspace(*str))
        str++;
    return str;
}

char *rtrim(char *str){
    char *back = str + strlen(str);
    while (isspace(*--back))
        *(back + 1) = '\0';
    return str;
}

char *trim(char *str){
    return rtrim(ltrim(str));
}

char* read_file(char* path){
    char *text;
    int fd, length, rbytes;
    if((fd = open(path, O_RDONLY, 0664)) == -1){
        perror("open");
        return NULL;
    }
    if((length = lseek(fd, 0, SEEK_END)) == -1){
        perror("seek to end\n");
        close(fd);
        return NULL;
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
		perror("seek to begining");
		close(fd);
		return NULL;
	}
    if((text = (char*)malloc(sizeof(char) * length)) == NULL){
		close(fd);
        return NULL;
    }
    if((rbytes = read(fd, text, length)) == -1){
        perror("read failed");
        //free(path);
        free(text);
		close(fd);
		return NULL;
    }
    close(fd);
    text[rbytes] = '\0'; //delete all binary data or line breaks that might have been read
    return text;         //because read also read bin data
}

/*converts string to num*/
float convertStrToNum(char* text){
    int i, count=1;
    float sum = 0;
    for(i = strlen(text) -1; i >=0; i--){
        sum += ((int)(text[i])-48) * count;
        count *= 10;
    }
    return sum;
}

char* getQty(char *order, int delimeter){
    // order is of the sort "Item Name Quantity"
    char *ch, *quantity;
    ch = strrchr(order, delimeter); // 32 is space's ascii
    if ((quantity = (char *)malloc(sizeof(char) * strlen(ch))) == NULL)
        return NULL;
    strncpy(quantity, ch + 1, strlen(ch));
    quantity[strlen(ch)]='\0';
    return quantity;
}

/*returns item that client typed in*/
char* getItem(char* order, int delimeter){
    char *ch, *item;
    ch = strrchr(order, delimeter); // 32 is space's ascii
    if ((item = (char *)malloc(sizeof(char) * (strlen(order)-strlen(ch)))) == NULL) //allocate the size of the item's name
        return NULL;
    strncpy(item, order, strlen(order) - strlen(ch));
    item[strlen(order) - strlen(ch)] = '\0';
    return item;
}

int write_to_file(int fd, char* str, int length){
    int written = 0, wbytes;
    while(written < length){
        if((wbytes = write(fd,str + written, length - written)) < 0) //write one after the other
            return -1;
        written += wbytes;
    }
    return 0;
}