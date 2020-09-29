# include <stdio.h>
# include <cs50.h>

int main(void)
{
    long int x = 4062901840;
    long int z = (x % 10000000000000) / 1000000000000;
    printf("%li\n", z);
}