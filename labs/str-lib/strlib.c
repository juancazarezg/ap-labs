#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str){

    int c = 0;
    while (str[c] != '\0'){
      c++; 
    } 
   return c;
}

char *mystradd(char *origin, char *addition){
    int newsize = mystrlen(origin) + mystrlen(addition);
    char *res = malloc(newsize + 1);
    int c, d;
   
   c = 0;
 
   while (origin[c] != '\0') {
       res[c] = origin[c];
      c++;      
   }
 
   d = 0;
 
   while (addition[d] != '\0') {
      res[c] = addition[d];
      d++;
      c++;    
   }

    return res;
}

int mystrfind(char *origin, char *substr){
    int c = 0;
    int size = mystrlen(substr);
    int s = 0;
    int k = 0;

    while(origin[s] != '\0'){
        if(origin[s]==substr[c]){
            c++;
            if(c == 1){
                k = s;
            }
            if(c == size){
                break;
            }
        }else{
            c = 0;
        }
        s++;
    }

    if (size == c){
        printf("['%s'] string was found at %d\n",substr, k);
    }else{
        printf("Ths string was not found \n");
    }

    return 0;
}



