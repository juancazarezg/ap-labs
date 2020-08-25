#include <stdio.h>
#include <stdlib.h>
/* print Fahrenheit-Celsius table */


int main(int argc, char *argv[])
{
    int fahr;

    for (fahr = atoi(argv[1]); fahr <= atoi(argv[2]); fahr = fahr + atoi(argv[3]))
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    return 0;
}
