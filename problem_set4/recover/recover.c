#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    // Check for correct CL argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open file received from CL or inform user of error
    FILE *recovery = fopen(argv[1], "r");
    if (!recovery)
    {
        printf("%s can not be opened for reading.\n", argv[1]);
        return 1;
    }

    // Create buffer for bytes
    typedef uint8_t BYTE;
    BYTE buffer[512];
    // Create file to write jpg output
    FILE *output;
    // Bool to check if first jpg file
    bool found_first_jpg = false;
    // Counter for amount of images
    int imageCounter = 0;

    // Keep looping until bytes can't be read from recovery file (fread() returns int value)
    while (fread(buffer, 512, 1, recovery) != 0)
    {
        // Bool which checks for specific .jpg first 4 bytes of buffer
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (imageCounter > 0)
                fclose(output);

            // Create filename and adjust counter
            char fileName[8];
            sprintf(fileName, "%03i.jpg", imageCounter++);

            // Create new file to store jpg
            output = fopen(fileName, "w");
            fwrite(buffer, 512, 1, output);

        }
        else if (imageCounter > 0)
        {
            // Keep writing until new jpg file found
            fwrite(buffer, 512, 1, output);
        }
    }
    // Close files after looping through entire recovery file
    fclose(output);
    fclose(recovery);
}
