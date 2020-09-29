# include <stdio.h>
# include <cs50.h>
# include <math.h>


long int get_positive_long(string prompt);


int main(void)
{
    // prompt for credit card num
    long int card_num = get_positive_long("Card number: "); 

    // values for for loop to go through the cc and add(and*) one set of num and add up the other set
    int rev_cc1 = 0;
    int rev_cc2 = 0;
    int mult = 0;
    long int m = 10; // to module the cc by
    long int n = 1; // to devide the moduled cc

    for (int i =0; i < 9; i++)
    {
        for(int j = 0; j < 1; j++ )
        {
            long int num1 = (card_num % m) / n; //this loop takes odd from last (and adds to rev_cc1)
            rev_cc1 = num1 + rev_cc1;
        }
        m = m*10; // making the for loop move up a num
        n = n*10;

        for(int k = 0; k < 1; k++ )
        {
            long int num2 = (card_num % m) / n; // this loop takes even from last (*2 and adds to rev_cc2)
            mult = num2*2;
            rev_cc2 = mult + rev_cc2;
        }
        m = m*10; // again making for loop move up one num
        n = n*10;
    }
    int rev_cc3 = rev_cc1 + rev_cc2; // adding rev_cc1 to rev_cc2
    
    // getting first num of module to check the type of card
    long int visa1 = (card_num % 10000000000000000) / 1000000000000000;
    long int visa2 = (card_num % 10000000000000) / 1000000000000;
    long int am_ex = (card_num % 1000000000000000) / 10000000000000;
    
    if (rev_cc3 % 10)
    {
        if (visa1 == 4)
        {
            printf("VISA\n"); // visa with 16 num 
        }
         if (visa2 == 4) // visa with 13 num
        {
            printf("VISA\n");
        }
        if (visa1 == 5) // mastercard with 16 num
        {
            printf("MASTERCARD\n");
        }
        if (am_ex == 34) // AmEx with 34 as first num
        {
            printf("AMEX\n");
        }
        if (am_ex == 37) // AmEx with 37 as first num
        {
            printf("AMEX\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}

// making sure cc num is positive not letter.. keep asking..
long int get_positive_long(string prompt)
{
    long int cc;
    do
    {
        cc = get_long("%s", prompt);
    }
    while (cc < 0.1);
    return cc;
}