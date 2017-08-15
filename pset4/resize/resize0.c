/**
 * Copies a BMP piece by piece, increasing by a factor of n, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // remember size
    int size = atoi(argv[1]);
    
    fprintf(stdout, "Resizing to a factor of %i.\n", size);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // write outfile's BITMAPFILEHEADER
    BITMAPFILEHEADER *bf_new = malloc(sizeof(BITMAPFILEHEADER));
    memcpy(&bf_new, &bf, sizeof(BITMAPFILEHEADER));
    
    // write outfile's BITMAPINFOHEADER
    BITMAPINFOHEADER *bi_new = malloc(sizeof(BITMAPINFOHEADER));
    memcpy(&bi_new, &bi, sizeof(BITMAPINFOHEADER));
    
    // determine padding for outfile lines
    int padding_new = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // update outfile's headers
    bi_new->biWidth = bi.biWidth * size;
    bi_new->biHeight = abs(bi.biHeight) * size;
    bi_new->biSizeImage = sizeof(RGBTRIPLE) * (bi_new->biWidth + padding_new) * bi_new->biHeight;
    bf_new->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi_new->biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // determine padding for infile scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over outfile's lines
        for (int m = 0; m < size; m++)
        {
            // if this isn't the first line, backup SEEK_CUR
            if (m > 0)
            {
                fseek(inptr, -1 * sizeof(RGBTRIPLE) * (bi.biWidth + padding), SEEK_CUR);
            }
            
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // iterate over pixels replicated in outfile
                for (int n = 0; n < size; n++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
    
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int k = 0; k < padding_new; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // free memory
    free(bf_new);
    free(bi_new);

    // success
    return 0;
}
