#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);

int main(int argc, char *argv[]) {
    int year =atoi(argv[1]);
    int day = atoi(argv[2]);

    char * months[12] ={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

    int *monthp, *dayp, dayc, monthc;
    dayc = 0;
    monthc = 0;
    dayp = &dayc;
    monthp = &monthc;

    int bisiesto;
    if(year % 100 == 0) {
        if(year %400 == 0 ){
            bisiesto = 1;
        }else{
            bisiesto = 1;
        }
    }else if(year % 4 == 0){
        bisiesto = 1;
    }else{
        bisiesto = 0;
    }

    if(day<1 || ((day>365 && !bisiesto) || (day>366 && bisiesto))){
        printf("El mes o día es inválido %d, %d\n", day,year);
    }else{
        int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
        int done = 0;
        for(int i=0;i<12;i++){
            if(done) break;
            if(day>days[i]+(i==1?bisiesto:0)){
                day -= days[i]+(i==1?bisiesto:0);
            }else{
                *monthp = i+1;
                *dayp = day;
                done = 1;
            }
        }

        printf("%s %d, %d\n", months[*monthp], *dayp,year);
    }

    

    return 0;
}
