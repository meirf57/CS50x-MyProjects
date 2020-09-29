#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2 || argc > 2) //checking to make sure a memory card was given
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    typedef uint8_t BYTE; // making a byte unit to measure later

    FILE *memory_c = fopen(argv[1], "r"); // open and read the mc given

    int size = 512;   // setting size of unit\block to 512
    unsigned char buffer[size + 1]; //buffer to read and later append images from
    char img_name[8]; // name of recovered image
    int i = -1; // setting a counter to name images
    FILE *img = NULL;


    while (fread(buffer, sizeof(BYTE), size, memory_c))
    {
        printf("%i\n", buffer[i]);
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            fclose(img);
            img = NULL;
        }

        else if (img)
        {
            goto add_to_img; // skipping to add stage of loop
        }

        printf("found\n"); // testing code*
        sprintf(img_name, "%03i.jpg", i); // changing the name for new img
        i++;
        printf("%s\n", img_name); // testing code*
        img = fopen(img_name, "w"); // opening img file
add_to_img:
        fwrite(buffer, sizeof(BYTE), size, img); // making new image in file (from found img)

    }

    remove("-01.jpg"); // removing the negative file that was formed prior to finding 1st img
    fclose(img); // closing last img
    fclose(memory_c); // closing mc
}
