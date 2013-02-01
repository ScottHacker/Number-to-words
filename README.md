Number-to-words
===============

![Numbers to Words in UNIX](http://iam.colum.edu/students/Scott.Hacker/blog/flatpress/fp-content/images/numberstowords.png)

I had heard that this deceptively easy programming assignment had been used quite a bit in interview situations.  I decided to try it myself and see how I would solve it.  I also took the opportunity to break away from C# and write something in C.

The idea behind this program is to take an integer (I limit it to non-negative ones, though I could have probably supported negative integers without too much trouble), then convert it into English words in proper format.  This sounds simple, but you quickly realize that the English language has pretty strange ways of formatting words that does not mesh well with computer processes.

[Source](https://github.com/ScottHacker/Number-to-words/blob/master/NumbersToWords.c)

How I did it
-------------

I started off by analyzing how we would say a variety of numbers and recognized that the English language breaks numbers into semantic groups of three which are always treated the same way.  i.e.: 123 and 123,000 are pronounced the same way, except that the latter has "thousand" on the end to denote it's grouping.  

So my first task was to make a function that could take groups of three numbers and put them into words.  During this, I realized that there would be no way to write out the numbers without writing it out entirely.  I found that all you needed was the numbers 1-19 and then all the numbers from 20-90 that are divisible by 10.

To start off with, I check if the number is over 20 and isn't divisible by 10, if so then I make it that way.  For example, passing in 26 will result in 20.  This way it fits into the word format expected for my upcoming case statement that will put the numbers into words.

    _Bool hasModulus = 0;
    if(n > 20 && n % 10 != 0){
      n -= n % 10;
      hasModulus = 1;
    }

Afterwards I made a large case statement to check for equality in the number and find the proper word for it, trying to simplify the statement where I could.  Here's where some of the weirdness of English comes in: 11 and 12 have their own words, but every word after that until 19 ends with teen.  Additionally, some are the 1-9 word plus teen (like Fourteen) while others are modified (like Thirteen).

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

Instead of giving a complete "Thirteen" or "Fourty" I get prefixes like "Thir" and "Four", this lets me reuse them and shorten this massive case statement a bit.  Now I just need to finish them by checking the number.  If it's less than 20 but over or equal to 13, then it needs a "teen".  Otherwise, if it's over or equal to 20, then we add a "ty".  This is also where the modulus bool I tracked comes in, if it had it, then I add a hyphen in expectation that the other number will be tacked on next.  In this case, I return a 1 instead of a 0, which will note to the function calling this that we're not done so we can grab the last number.

    if(n < 20 && n >= 13){
      printf("teen");
    }
  
    if(n >= 20 ){
      printf("ty");
      if(hasModulus){
        printf("-");
        return 1;
      }
    }
    return 0

Now I have a function that can more or less translate every number into English, all I have to do is build a new function, I called "Speak", to send in the numbers and construct the "glue" between them, that is, the "and"s, commas, and "hundreds"/"thousands"/"millions".  I do this by taking the number passed in then breaking it into groups of three and passing them into the previous NumberToWord function.

I start out by checking if it's zero, if it is, it just prints out the word and returns.  I did this because the way I set up the previous function, if I had added a Zero to that case statement, then passing in a number like 5,000 would get me the word "Five thousand and Zero".

    if(t == 0) {
      printf("Zero.");
      return 0;
    }

If it's not zero, then we get into the function proper.  We start by getting the length of the integer using another function.  This is kinda like getting the length of a string, an integer like 12345 will return 5.

      int length = GetLength(t);
      
    int GetLength(int n){
      int length=1;
      while(n>9){ length++; n/=10; }
      return length;
    }

Once I've got the length, I run a loop for every group of 3 in that loop.  It casts it into an integer, so any floating points would get chopped off (i.e.: 2.6 will be 2).  I subtract by 0.1 so that it'll round down even if it divides evenly, so that the responses can be used later in power-of-n math.  I get each "group" of the number by using length divided by 1000 to the power of the current group I'm on.  For example, on the second iteration of the loop for number 123,456,789, the "group" integer would equal "456".

    for (int g = length/3.0 - 0.1; g >= 0; g--){
      int group = t / pow(1000, g);

Once I've got that number, I remove it from the main number.  Since this loop goes from left to right, I'd be slowly "eating" the number from left to right, so that a number like 123,456,789 would on the first loop become 456,789, on the second become 789, and on the third (and last) become 0.

    t -= group * pow(1000,g);

Then I figure out how large the group is.  Even though I split it into groups of 3, one of the groups may be made out of 1 or 2 numbers instead of 3.  For example: 12,345 would give me the groups 12 and 345.  So I check to see how large the group is, then give a number which I'll use in some math later on.

    int i;
    if(group  >= 100)
      i = 100;
    else if (group >= 10)
      i = 10;
    else if (group >= 1)
      i = 1;

Finally, all of our setup is done.  Now we do the actual work of constructing the number into English words.  I make a loop that will run until the number we created in the last bit of code, "i", is equal to 0.  It does this by getting modulus of the group divided by i so that it drops the right most numbers off of it, then I decrement the "i" count by dividing by 10.  This will allow me to break the group of 3 into individual numbers that I can send to the other function the "NumberToWord" function that I made previously.

There's some extra "glue" work to do here as well: if the group is larger or equal to 100, then I need to tack on the word "Hundred", if it's more than 100, then I also need an "and" to bridge to the next numbers.

Otherwise, if the group is less than 100, I send the entire thing to the NumbersToWord function and see what happens.  I've got 2 numbers left at this point, and depending on the number I could either be done (in the case of "twelve") or not (in the case of "thirty-").  If it returns a 0, then I'm done and it breaks out of the loop, otherwise if it returns a 1, I'm not, and it continues on to the last number of the group.

    while( i != 0){
      if(i == 100){
        NumberToWord(group/i);
        if(group/i > 0)
          printf(" Hundred");

        if(group % i > 0)
          printf(" and ");
      }
      else{
        if(NumberToWord(group) == 0)
          break;
      }

      group = group % i;
      i/= 10;
    }

We're almost done, at the last part of the loop that splits the number into groups of 3.  We need a bit more glue based on which group we're on, to say "thousand", "Million", or "billion".  I could have added more, but the standard signed integer I'm using to store numbers only goes up to 2,147,483,647 which I figure is more than enough for my purposes, so there's no point going above "billion".  

So I check which run of the loop we're in and add on one of the numbers based on it.  I also check the main number to see if the remaining piece is over or equal to 100, if it is then we need a comma to bridge between the word groups.  If not, but the remaining piece is over zero, then we need an and.  This is the difference between 3,456 equaling "Three Thousand, Four Hundred and Fifty-Six" and 3,001 equaling "Three Thousand and One".  Once we're done with that, we're finally done with the loop.

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

      if(t >= 100)
        printf(", ");
      else if(t > 0){
        printf(" and ");
      }
    }
  }

Phew, finally done.  All we have to do now is add a period to the end of the sentence to be grammatically correct then return 0 to denote that everything finished alright.

    printf(".");
    return 0;
  }

Now all we need is a main function to take in a number from the user and run our previous function.

    int main (void)
    {
      int n;
    
      scanf ("%i", &n);
    
      Speak(n);
      
      return 0;
    }

Done!  That was quite a bit more difficult than it sounded, but pretty interesting to figure out.
