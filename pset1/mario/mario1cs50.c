# include <stdio.h>
# include <cs50.h>


int get_height_int(string prompt);


int main(void)
{
   int height = get_height_int("Height: ");
   
   for(int x = 0; x <= height; x += 1 )
   {
       for(int z = height; z > x; z -= 1)
       printf(" ");
       for(int y = 1; y <= x; y += 1)
       {
           printf("#");
       }
       printf("\n");
   }
}



int get_height_int(string prompt)
{
    int num;
    do
    {
        num = get_int("%s", prompt);
    }
    while (num<1 || num>8);
    return num;
}
