#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    //Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    //Open file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }

    bool finished = false;
    int count = -1;
    int iter = 0;
    int position = 0;
    FILE *image;

    while (finished == false)
    {
        //Read the current 512 block
        unsigned char buffer[512];
        size_t size = fread(buffer, 512, 1, file);

        //If we have reached the end of the file, break out of loop
        if (size != 1)
        {
            fclose(image);
            finished = true;
        }

        //Check to see if the 512 block signals beginning of a jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            count += 1;

            //If the first one, declare & open image
            if (count == 0)
            {
                char image_name[8];
                sprintf(image_name, "%03u.jpg", count);
                image = fopen(image_name, "a");
            }

            //If not the first, close the previous image
            else
            {
                fclose(image);
                char image_name[8];
                sprintf(image_name, "%03u.jpg", count);
                image = fopen(image_name, "a");

            }

            //We have opened the images in append mode so writing will add to the end of the file
            fwrite(buffer, 512, 1, image);
        }

        //If not the beginning of a new jpg, it is a continuation of an old jpg so write to already open image
        else
        {
            //Check an image is open
            if (count != -1)
            {
                fwrite(buffer, 512, 1, image);
            }
        }

        //Increase iteration to jump to next 512 block
        iter += 1;

        //Set position in file to next block of 512 bytes
        fseek(file, 512 * iter, SEEK_SET);
    }
    
    return 0;
}
