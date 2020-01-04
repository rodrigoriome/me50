#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define CHUNK_SIZE 512

// Unsigned 8 bit integer
typedef uint8_t BYTE;

// Keep track of how many images we recovered
int images_found;
bool file_is_open;

bool is_jpeg_head(BYTE *buffer);
FILE *make_img_file();

int main(int argc, char *argv[])
{
    // Validate arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open memory card
    FILE *memcard_ptr = fopen(argv[1], "r");
    if (memcard_ptr == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    images_found = 0;
    file_is_open = false;

    FILE *output_ptr = NULL;
    BYTE data_chunk[CHUNK_SIZE];

    // Iterate until all blocks are processed
    while (fread(data_chunk, CHUNK_SIZE, 1, memcard_ptr) != 0)
    {
        if (is_jpeg_head(data_chunk))
        {
            if (file_is_open)
            {
                fclose(output_ptr);
                file_is_open = false;
            }

            output_ptr = make_img_file();
            images_found++;
            file_is_open = true;
            fwrite(data_chunk, CHUNK_SIZE, 1, output_ptr);
        }
        else
        {
            if (file_is_open)
            {
                fwrite(data_chunk, CHUNK_SIZE, 1, output_ptr);
            }
        }
    }

    fclose(output_ptr);
}

// Check for JPEG headers in data buffer
bool is_jpeg_head(BYTE *buffer)
{
    if (
        buffer[0] == 0xff
        && buffer[1] == 0xd8
        && buffer[2] == 0xff
        && (buffer[3] & 0xf0) == 0xe0
    )
    {
        return true;
    }

    return false;
}

// Generate a new file and return its pointer
FILE *make_img_file()
{
    char filename[8];
    sprintf(filename, "%03i.jpg", images_found);
    FILE *new_file = fopen(filename, "w");

    return new_file;
}
