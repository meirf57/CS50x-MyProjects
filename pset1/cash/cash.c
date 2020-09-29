# include <stdio.h>
# include <cs50.h>
# include <math.h>


float get_positive_float(string prompt);


int main(void)
{
    int q = 25;  // q is quarter value
    int d = 10;  // d is dime value
    int n = 5;  // n is nickel value
    int p = 1;  // p is penny value

    // getting the float to calculate change
    float cash = get_positive_float("Owe: ");

    // making float given into int fo further calcs
    int coins = round(cash * 100);

    // calculating how many quarters
    int c1 = 0;
    for (int ch1 = coins; ch1 > 24; ch1 -= q)
    {
        (c1++);
    }

    // calculating dimes (minus quarters)
    int c2 = 0;
    for (int ch2 = coins - c1 * q; ch2 > 9; ch2 -= d)
    {
        (c2++);
    }

    // calculating nickels [minus (quarters + dimes)]
    int c3 = 0;
    for (int ch3 = coins - (c1 * q + c2 * d); ch3 > 4; ch3 -= n)
    {
        (c3++);
    }

    // calculating pennys [minus (quarters + dimes + nickels)]
    int c4 = 0;
    for (int ch4 = coins - (c1 * q + c2 * d + c3 * n); ch4 > 0; ch4 -= p)
    {
        (c4++);
    }

    // adding amount of coins for change
    int change = (c1 + c2 + c3 + c4);

    printf("%i\n", change);
}


// making sure the float given is above 0
float get_positive_float(string prompt)
{
    float num;
    do
    {
        num = get_float("%s", prompt);
    }
    while (num < 0.001);
    return num;
}