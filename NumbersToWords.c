#include <stdio.h>

int main (void)
{
  int n;

  printf ("Please type in a non-negative integer \n");
  scanf ("%i", &n);

  Speak(n);
  printf("\nNumber after function: %i", n);
  
  //printf("%i", &n % 10);

  /* do{ */
  /*   printf("%i", n % 10); */
  /*   n /= 10; */
  /* }while(n != 0); */

  return 0;
}

int Speak(int t)
{
  int i = 100;

  while( i != 0){
    printf("%i ", t/i);
    if(i == 100 && t/i > 0)
      printf(" hundred");

    t -= t/i * i;
    i/= 10;
  }
  /* for(int i = 1000, n = t; i != 0; i/=10, n /= i){ */
  /*   printf("%i ", n); */
  /* } */
  return 0;
}
