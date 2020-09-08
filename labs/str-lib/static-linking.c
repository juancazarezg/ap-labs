#include <stdio.h>
#include <string.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char *argv[]) {

    if (argc < 3)
    {
        printf("Not enough arguments.\n");
    }else
    {
        if(strcmp(argv[1],"-find")){
            printf("Initial Lenght: %d\n",mystrlen(argv[2]));
            printf("New String: %s\nNew length: %d\n",mystradd(argv[2],argv[3]),mystrlen(mystradd(argv[2],argv[3]))); 
        }
        if(strcmp(argv[1],"-add")){
             if (mystrlen(argv[2]) < mystrlen(argv[3]))
            {
                printf("The substring is bigger\n");
            }
            else
            {
                mystrfind(argv[2],argv[3]);
            }   
        }

        
    }

    return 0;
}
