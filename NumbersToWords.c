#include <stdio.h>
#include <math.h>

int main (void)
{
  int n;

  printf ("Please type in a non-negative integer \n");
  scanf ("%i", &n);

  //This does everything.
  Speak(n);
  
  return 0;
}

//Translates an non-negative integer into words
int Speak(int t)
{
  //If it's zero, let's just print it right out.
  if(t == 0) {
    printf("Zero.");
    return 0;
  }

  //Get the length, then loop for each group of 3 in that length
  //I subtract it by 0.1 so that it always rounds down, even if it
  //divides evenly.  This is so that I get responses that can be used
  //as a power of, i.e.: 123 gets 0, which would make pow(1000, 0)
  //equal 1.
  int length = GetLength(t);
  for (int g = length/3.0 - 0.1; g >= 0; g--){
    //Get the numbers that comprise each group of three
    //For example, 12345 would get 12 and 345
    int group = t / pow(1000, g);

    //Now that we've got the group stored, let's get it out of the
    //main number right now.
    t -= group * pow(1000,g);

    //Figure out how large the group is
    int i;
    if(group  >= 100)
      i = 100;
    else if (group >= 10)
      i = 10;
    else if (group >= 1)
      i = 1;

    //Go through each number in the group and translate it to a word properly.
    while( i != 0){
      //If it's in the hundreds, then we need to add "Hundred" and
      //possibly "and" afterwards if there's more.
      if(i == 100){
        NumberToWord(group/i);
        if(group/i > 0)
          printf(" Hundred");

        if(group % i > 0)
          printf(" and ");
      }
      else{
        //If it's anything else, then let's just say it.  We check if
        //it's zero, which indicates that we're done with this loop.
        if(NumberToWord(group) == 0)
          break;
      }

      //Remove the left-most number from the number
      group = group % i;

      //decrement the count by dividing by 10.
      i/= 10;
    }

    //Figure out what the appropriate tag for this group is
    if(group != 0){
      switch(g){
      case 1:
        printf(" Thousand");
        break;
      case 2:
        printf(" Million");
        break;
      case 3:
        printf(" Billion");
        break;
      }

      //(t/ pow(1000, g - 1)) >= 100
      //If we're not done, add a comma.
      if(t >= 100)
        printf(", ");
      //If all that's left is less than 100 but greater than 0, then we need an 'and'.
      //For example: 5001 should equal "Five-Thousand and One".
      else if(t > 0){
        printf(" and ");
      }
    }
  }
  //We're done!  Add a period and call it a day.
  printf(".");
  return 0;
}

//Takes a number under 100 and turns it into an English word
//Returns 0 to indicate that it's done, and 1 to continue
int NumberToWord(int n){
  //Check if the number is over 20 and has a remainder if divided by
  //10.  This means that we'll have to treat it a little different.
  _Bool hasModulus = 0;
  if(n > 20 && n % 10 != 0){
    n -= n % 10;
    hasModulus = 1;
  }

  //The actual statement to figure out the words to use.
  //Tried to minimize the cases as much as possible, accounting for
  //redundancies in the teens and over 20 numbers.
  switch(n)
    {
    case 1:
      printf("One");
      break;
    case 2:
      printf("Two");
      break;
    case 3:
      printf("Three");
      break;
    case 4:
    case 14:
    case 40:
      printf("Four");
      break;
    case 5:
      printf("Five");
      break;
    case 6:
    case 16:
    case 60:
      printf("Six");
      break;
    case 7:
    case 17:
    case 70:
      printf("Seven");
      break;
    case 8:
      printf("Eight");
      break;
    case 9:
    case 19:
    case 90:
      printf("Nine");
      break;
    case 10:
      printf("Ten");
      break;
    case 11:
      printf("Eleven");
      break;
    case 12:
      printf("Twelve");
      break;
    case 13:
    case 30:
      printf("Thir");
      break;
    case 15:
    case 50:
      printf("Fif");
      break;
    case 18:
    case 80:
      printf("Eigh");
      break;
    case 20:
      printf("Twen");
      break;
    }

  //Finally, if the number is less than 20 but over or equal to 13, we
  //add "teen" to the word.
  if(n < 20 && n >= 13){
    printf("teen");
  }
  //If the number is over or equal to 20, then we add "ty"
  //Also, add a hyphen if we need to run again for the remainder.
  if(n >= 20 ){
    printf("ty");
    if(hasModulus){
      printf("-");
      return 1;
    }
  }
  return 0;
}

//Gets the "length" of a number, i.e.: 12345 will equal 5, for being
//comprised of 5 numbers.
int GetLength(int n){
  int length=1;
  while(n>9){ length++; n/=10; }
  return length;
}
