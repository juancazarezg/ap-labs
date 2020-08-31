#include <stdio.h>
#include <string.h>

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */
#define ARRAY_LENGHT 80

void reverse(char* word)
{
   int size = strlen(word);
   char* a = word;
   char* b = word + size - 1;
 
   for (char c = 0; c < size/2; c++)
   {        
      *a = *a + *b;
      *b = *a - *b;
      *a = *a - *b;
      a++;
      b--;
   }
}

int main(){
    // Place your magic here
    char c;
    char reversed[ARRAY_LENGHT] = "";
    char tmp;

    while ((c = getchar()) != EOF) {
      scanf("%s", reversed);
	   if (c == '\n'){
        reverse(reversed);
        printf("Reversed word: %s \n", reversed);
        reversed[0] = '\0';
      }
    }
    return 0;
}


