#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

bool isJpgBlock(uint8_t *block);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        fprintf(stderr, "Where image is a disk image to be examined for JPEG files.\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[1];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // set buffer to size of 1 block of 512 bytes
    uint8_t *buffer = malloc(512);
    
    // set blockSize and read first block
    int blockSize = fread(buffer, 1, 512, inptr);
    
    // set housekeeping variables
    bool inJpg = false;
    char filename[] = "000.jpg";
    int filenum = 0;
    
    // set FILE variable
    FILE *outptr;
    
    // go through until some block has a size != 512 - also kill at INT_MAX to prevent inf loop
    for (int i = 0; blockSize == 512 && i < INT_MAX; i++)
    {
        // check if we're starting new block
        if (isJpgBlock(buffer))
        {
            // set next file name
            if (filenum > 0)
                sprintf(filename, "%03i.jpg", filenum);
            
            // open new file for writing
            outptr = fopen(filename, "w");
            
            // housekeep
            filenum++;
            inJpg = true;
        }
        
        // write if we're in a jpg (this skips first blocks)
        if (inJpg)
            fwrite(buffer, 512, 1, outptr);

        // get next block
        blockSize = fread(buffer, 1, 512, inptr);
    }
    
    fclose(outptr);
    free(buffer);
}

// check leading bytes of a 512 byte block
// to see if it's the beginning of a jpg file
bool isJpgBlock(uint8_t *block)
{
    if (block[0] == 0xff &&
        block[1] == 0xd8 &&
        block[2] == 0xff &&
        (0xe0 <= block[3] && block[3] <= 0xef))
    {
        return true;
    }
    
    return false;
}