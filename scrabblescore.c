/* Scrabble Score calculator
 * 
 * calculates the scrabble score for a given word
 * 
 * Brenda Anderson, 11 Sep 2016
 */

#include <stdio.h>

// Prototype 
int calculate_score(char* p);

int main(int argc, char* argv[])
{
    // make sure user entered a word
    if (argc != 2)
    {
        printf("usage: ./scrabblescore word\n");
        return 1;
    }
    int score = calculate_score(argv[1]);
    
    // print the answer
    if (score != 0)
        printf("%s is worth %i points\n", argv[1], score);    
    else
        printf("invalid entry\n");
    
    return 0;
}

int calculate_score (char* p)
{
   int counter = 0;
   // score values                 a b c d e f g h i j k l m n o p  q r s t u v w x y  z
   static const int scores[26] = { 1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10 };

   // iterate over the word
    while (*p)
    {
        // calculate the score for each letter
        if (*p >= 'a' && *p <= 'z')
        {
            counter += scores[(*p) - 'a'];
        }
        else if (*p >= 'A' && *p <= 'Z')
        {
            counter += scores[(*p) - 'A'];
        }
        else
        {
            return 0;
        }
        p++;
    }
    return counter;
    
    
}