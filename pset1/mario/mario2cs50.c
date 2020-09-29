# include <stdio.h>
# include <cs50.h>


int get_height_int(string prompt);



int main(void)
{
    int height = get_height_int("Height: ");

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
        
        printf(" "); // making the middle column
        
        // for loop for pyramid left down to right
        
        for (int y = 1; y <= x; y += 1) // y is column
        {
            printf("#");
        }
        
                for (int z = height; z > x; z -= 1) // z is the space in row
        {
            printf("0"); 
        }
        
        printf("\n");
    }
}


// creating a while loop so that the input is 1-8

int get_height_int(string prompt)
{
    int num;
    do
    {
        num = get_int("%s", prompt);
    }
    while (num < 1 || num > 8);
    return num;
}