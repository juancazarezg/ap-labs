#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


//Function to merge
void merge(void *array[], int l, int m, int r, int (*comp)(void *, void *)){
    int leftSize = m - l + 1;
    int rightSize = r - m;

    char *left[leftSize], *right[rightSize];

    for (int i = 0; i < leftSize; i++){
        left[i] = array[l + i];
    }

    for (int i = 0; i < rightSize; i++){
        right[i] = array[m + i + 1];
    }
    
    int i = 0, j = 0,k = l;

    while (i < leftSize && j < rightSize){
        if ((*comp)(left[i], right[j]) < 0){
            array[k] = left[i];
            i++;
        } else{
            array[k] = right[j];
            j++;
        }
        k++;
    }
    while (i < leftSize){
        array[k] = left[i];
        i++;
        k++;
    }

    while (j < rightSize){
        array[k] = right[j];
        j++;
        k++;
    }


}

//Function to sort
void mergeSort(void *array[], int l, int r, int (*comp)(void *, void *)){
    if (l < r){
        int mid = (l + r) / 2;

        mergeSort(array, l, mid, (*comp));
        mergeSort(array, mid + 1, r, (*comp));

        merge(array, l, mid, r, (*comp));
    }
}

//Function to convert strings to numbers and then compare them
int numcmp(const char *s1, const char *s2) {
    double v1 = atof(s1);
    double v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

int main(int argc, char** argv)
{
    FILE *file;
    char **array;
    int i = 0;
    char next[100]; 
    int word_alloc = 4; 
    array = (char **) malloc(sizeof(char*) * word_alloc);

    if (argc == 3 && strcmp(argv[2], "numbers.txt") == 0){
        int numeric = 1;
        
        if ((file = fopen("numbers.txt", "r")) == NULL){
        printf("Error opening file");
        return 0;
        }
        while (fscanf(file, "%s", next) != EOF){
            if (i > word_alloc){
                word_alloc *= 2;
                array = (char **) realloc(array, sizeof(char*) * word_alloc);
            }
            array[i] = (char *) malloc(sizeof(char) * (strlen(next) + 1));
            strcpy(array[i], next);
            ++i;
        }
        mergeSort((void *) array, 0, 99, (int (*)(void*, void*)) (numcmp));
         
    } else if (argc == 2 && strcmp(argv[1], "strings.txt") == 0){
        int numeric = 0; 
        if ((file = fopen("strings.txt", "r")) == NULL){
        printf("Error opening file");
        return 0;
        }
        while (fscanf(file, "%s", next) != EOF){
            if (i > word_alloc){
                word_alloc *= 2;
                array = (char **) realloc(array, sizeof(char*) * word_alloc);
            }
            array[i] = (char *) malloc(sizeof(char) * (strlen(next) + 1));
            strcpy(array[i], next);
            ++i;
        }
        mergeSort((void *) array, 0, 99, (int (*)(void*, void*)) (strcmp));

    } else{
        printf("Use numbers.txt or strings.txt to run this program, \n");
        return 0;
    }

    for (int j = 0; j < i; j++){
        printf("%s \n", array[j]);
    }
    fclose(file);
    return 0;
}

