# include <stdio.h>
//# include <cs50.h>


int get_height_int(int *prompt);



int main(void)
{
    printf("Height: ");
    int height;
    scanf("%i", &height);
    while (height < 1 || height > 8)
    {
        printf("Height 1-8, Height: ");
        scanf("%i", &height);
    }
    

    for (int x = 1; x <= height; x += 1) // x is the row and z the space in row
    {
        // for loop for pyramid right down to left

        for (int z = height; z > x; z -= 1)
        {
            printf(" ");
        }

        for (int y = 1; y <= x; y += 1) // y is column
        {
            printf("#");
        }

// making the middle column
 
        printf("  ");

        // for loop for pyramid left down to right

        for (int y = 1; y <= x; y += 1) // y is column
        {
            printf("#");
        }

        printf("\n");
    }
}


// creating a while loop so that the input is 1-8

int get_height_int(int *prompt)
{
    int num;
    do
    {
        num = scanf("%i", &prompt);
    }
    while (num < 1 || num > 8);
    return num;
}