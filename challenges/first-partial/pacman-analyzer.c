#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#if !(defined SSIZE_MAX)
#define SSIZE_MAX (SIZE_MAX >> 1)
#endif

// Juan José Cazarez Gastelum A00570574

//Function for reading log file and writing info into report file
void analizeLog(char *logFile, char *report);
//My implementation of get line
ssize_t mygetline(char **plinebuf, size_t *psize, FILE *file);
//This function checks every line of the logFile and gets the data
void analizePack(char* lineB);
//Function for helping analizePack for getting the name of the package
char *getName(char *line);
//Function for helping analizePack for getting the date of the package
char *getDate(char *line);

//Initial counters
int installed, upgraded, removed;
int current = 0;

//The struct of the information collected by analizePack
struct info{
  char name[30];
  char instDate[30];
  char lastUp[30];
  int updts;
  char remove[30];
};
struct info packs[3000];


int main(int argc, char **argv) {

  if (argc != 5) {
	printf("Incorrect input, try: ./pacman-analyzer.o -input 'logFile.txt' -report 'reportFile.txt' \n");
	return 1;
  }

  analizeLog(argv[2], argv[4]);

  return 0;
}

//For this function we enter the buffered line and get the name of the package
char *getName(char *line){
  int c = 0, start = 0, size = 0;
    for (int i = 0; i < 2; i++){
        for (start; line[start] != ']'; start++);
        start +=2;
    }
    for (int i = 0; i < 2; i++){
      if(i==0){
        for (start; line[start] != ' '; start++);
      }else{
        for (int i = start+1; line[i] != ' '; i++){
        size++;
        }
      }
    }
    char *name = calloc(1, size);
    for (int i = start+1; line[i] != ' '; i++){
        name[c] = line[i];
        c++;
    }
    return name;
}

//For this function we enter the buffered line and get the date of the package
char *getDate(char *line){
  int size=0,c=0;
  for(size; line[size] != ']'; size++);
  char *date = calloc(1, size);
  for (int i = 1; i < size; i++){
    date[c] = line[i];
    c++;
  }
  return date;
}

//For this function we enter the buffered line and check if the package is being installed, removed or upgraded. After this we put the data on the packs struct
void analizePack(char* lineB){

  if(strstr(lineB,"] installed") !=NULL){

    strcpy(packs[current].name, getName(lineB));
    strcpy(packs[current].instDate, getDate(lineB));
    strcpy(packs[current].lastUp, getDate(lineB));
    packs[current].updts = 0;
    strcpy(packs[current].remove, "-");
    installed++;
    current++;

  }else if(strstr(lineB,"] upgraded") !=NULL){

    strcpy(packs[current].name, getName(lineB));
    strcpy(packs[current].instDate, getDate(lineB));
    for (int i = 0; i < current; i++){
      if (strcmp(packs[i].name, getName(lineB)) == 0){
        if (packs[i].updts == 0){
          upgraded++;
        }
        packs[i].updts++;
        strcpy(packs[i].lastUp, getDate(lineB));
        break;
      }  
    }
    
  }else if(strstr(lineB,"] removed") !=NULL){

    for (int i = 0; i < current; i++){
      if (strcmp(packs[i].name, getName(lineB)) == 0){
        strcpy(packs[i].remove, getDate(lineB));
        break;
      }
    }
    removed++;

  }
}

//In this function the log file is opened, we call the analizePack function after buffering each line and then with the collected data we write the results
//into the report file
void analizeLog(char *logFile, char *report) {
  printf("Generating Report from: [%s] log file\n", logFile);
  FILE *fileLog = fopen(logFile, "r");
  //use of open() function
  int fileReport = open(report, O_WRONLY | O_CREAT, 0644);
  char *linebuf = NULL;
  size_t linebuf_size = 0;
  int line_count = 0;
  size_t line_size;
  int n = 0;
  char ch;
  // Implement your solution here.
  if (!fileLog) {
    fprintf(stderr, "Error opening file '%s'\n", logFile);
    return;
  }
  if (fileReport == -1) { 
    printf("Error opening or creating file %s \n", report);
    return; 
  }

  while((ch = fgetc(fileLog)) != EOF){
		if(ch == '\n'){
			n++;
		}
	}
  fseek(fileLog,0,SEEK_SET);

  line_size = getline(&linebuf, &linebuf_size, fileLog);

  while (line_count<n){
    line_count++;
    analizePack(linebuf);
    line_size = getline(&linebuf, &linebuf_size, fileLog);
  }

  free(linebuf);
  linebuf = NULL;

  char sinstalled[10];
  char supgraded[10];
  char sremoved[10];
  char scurrentInstalled[10];
  int currentInstalled = installed-removed;
  sprintf(sinstalled,"%d", installed);
  sprintf(supgraded,"%d", upgraded);
  sprintf(sremoved,"%d", removed);
  sprintf(scurrentInstalled,"%d", currentInstalled);

  //use of write() function
  write(fileReport, "Pacman Packages Report\n", strlen("Pacman Packages Report\n"));
  write(fileReport,"----------------------\n",strlen("----------------------\n"));
  write(fileReport,"- Installed packages : ",strlen("- Installed packages : "));
  write(fileReport,sinstalled,strlen(sinstalled));
  write(fileReport, "\n", strlen("\n"));
  write(fileReport,"- Removed packages   : ",strlen("- Removed packages   : "));
  write(fileReport,sremoved,strlen(sremoved));
  write(fileReport, "\n", strlen("\n"));
  write(fileReport,"- Upgraded packages  : ",strlen("- Upgraded packages  : "));
  write(fileReport,supgraded,strlen(supgraded));
  write(fileReport, "\n", strlen("\n"));
  write(fileReport,"- Current installed  : ",strlen("- Current installed  : "));
  write(fileReport,scurrentInstalled,strlen(scurrentInstalled));
  write(fileReport, "\n", strlen("\n"));
  write(fileReport, "\n", strlen("\n"));
  write(fileReport, "List of packages\n", strlen("List of packages\n"));
  write(fileReport, "----------------\n", strlen("----------------\n"));
  
  for(int i = 0;i<current;i++){
    write(fileReport, "- Package Name        : ", strlen("- Package Name        : "));
    write(fileReport, packs[i].name, strlen(packs[i].name));
    write(fileReport, "\n", strlen("\n"));
    write(fileReport, "  - Install date      :  ", strlen(" - Install date      :  "));
    write(fileReport, packs[i].instDate, strlen(packs[i].instDate));
    write(fileReport, "\n", strlen("\n"));
    write(fileReport, "  - Last update date  :  ", strlen(" - Last update date  :  "));
    write(fileReport, packs[i].lastUp, strlen(packs[i].lastUp));
    write(fileReport, "\n", strlen("\n"));
    write(fileReport, "  - How many updates  : ", strlen("  - How many updates  : "));
    char temp[5];
    sprintf(temp, "%d", packs[i].updts);
    write(fileReport, temp, strlen(temp));
    write(fileReport, "\n", strlen("\n"));
    write(fileReport, "  - Removal date      :  ", strlen(" - Removal date      :  "));
    write(fileReport, packs[i].remove, strlen(packs[i].remove));
    write(fileReport, "\n", strlen("\n"));
  }
  //use of close() function
  close(fileReport);
  fclose(fileLog);

  printf("Report is generated at: [%s]\n", report);
}

//This is my implementation of the getLine function
ssize_t mygetline(char **plinebuf, size_t *psize, FILE *file)
{
  const size_t INITALLOC = 16;
  const size_t ALLOCSTEP = 16;
  size_t num_read = 0;

  if ((NULL == plinebuf) || (NULL == psize) || (NULL == file)){
    errno = EINVAL;
    return -1;
  }

  if (NULL == *plinebuf){
    *plinebuf = malloc(INITALLOC);
    if (NULL == *plinebuf){
      return -1;
    }
    else{
      *psize = INITALLOC;
    }
  }

  {
    int c;
    while ((c = getc(file)) != EOF){
      num_read++;
      if (num_read >= *psize){
        size_t n_realloc = *psize + ALLOCSTEP;
        char * tmp = realloc(*plinebuf, n_realloc + 1); 
        if (NULL != tmp){
          *plinebuf = tmp;
          *psize = n_realloc;
        }
        else{
          return -1;
        }
        if (SSIZE_MAX < *psize){
          errno = ERANGE;
          return -1;
        }
      }
      (*plinebuf)[num_read - 1] = (char) c;
      if (c == '\n'){
        break;
      }
    }

    if (EOF == c){
      errno = 0;
      return -1;
    }
  }
  (*plinebuf)[num_read] = '\0';

  return (ssize_t) num_read;
}