#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r"); // making a file of txt given

    if (f == NULL) // checking the file
    {
        return false;
    }

    size_t pos = ftell(f);    // Current position
    fseek(f, 0, SEEK_END);    // Go to end
    size_t length = ftell(f); // read the position which is the size
    fseek(f, pos, SEEK_SET);  // restore original position

    printf("len = %li\n", length);

    int count_lines = 0;
    char filechar[40], chr;

   //extract character from file and store in chr
    chr = getc(f);
    while (chr != EOF)
    {
        //Count whenever new line is encountered
        if (chr == 'n')
        {
            count_lines = count_lines + 1;
        }
        //take next character from file.
        chr = getc(f);
    }
    fclose(f); //close file.
    printf("There are %d lines in %s  in a file\n", count_lines, filechar);
    return 0;
}
