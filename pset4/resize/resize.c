/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

void copyBf(BITMAPFILEHEADER in, BITMAPFILEHEADER *out);
void copyBi(BITMAPINFOHEADER in, BITMAPINFOHEADER *out);

int main(int argc, char *argv[])
{
    int size = 1;
    
    if (argc == 4)
    {
        size = atoi(argv[1]);
    }
    
    // ensure proper usage
    if (argc != 4 || (size < 1 || 100 < size))
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        fprintf(stderr, "n must be a positive integer <= 100.\n");
        return 1;
    }
    
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
    
    // PREP INFILE

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
    
    // PREP OUTFILE
    
    // create outfile's BITMAPFILEHEADER
    BITMAPFILEHEADER bfNew;
    copyBf(bf, &bfNew);
    
    // create outfile's BITMAPINFOHEADER
    BITMAPINFOHEADER biNew;
    copyBi(bi, &biNew);
    
    // update outfile's headers, and make padding for outfile
    biNew.biWidth = bi.biWidth * size;
    biNew.biHeight = bi.biHeight * size;
    int padNew = (4 - (biNew.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    biNew.biSizeImage = (biNew.biWidth * sizeof(RGBTRIPLE) + padNew) * abs(biNew.biHeight);
    bfNew.bfSize = biNew.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // INSPECT HEADERS
    
    // Old file Specs
    fprintf(stdout, "\nbf.bfSize = %i, bi.biSizeImage = %i\n", bf.bfSize, bi.biSizeImage);
    fprintf(stdout, "sizeof(BITMAPFILEHEADER) = %lu, sizeof(BITMAPINFOHEADER) = %lu\n", sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
    fprintf(stdout, "bi.biWidth = %i, bi.biHeight = %i\n\n", bi.biWidth, bi.biHeight);
    
    // New file specs
    fprintf(stdout, "\nbfNew.bfSize = %i, biNew.biSizeImage = %i\n", bfNew.bfSize, biNew.biSizeImage);
    fprintf(stdout, "sizeof(BITMAPFILEHEADER) = %lu, sizeof(BITMAPINFOHEADER) = %lu\n", sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
    fprintf(stdout, "biNew.biWidth = %i, biNew.biHeight = %i\n\n", biNew.biWidth, biNew.biHeight);
    
    // BEGIN WRITING

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfNew, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biNew, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine offset for fseek
    long long offset = (long) -1 * ((long) bi.biWidth * (long) sizeof(RGBTRIPLE) + (long) padding);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int m = 0; m < size; m++)
        {
            if (m != 0)
            {
                // back up read cursor m-1 times
                fseek(inptr, offset, SEEK_CUR);
            }
            
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                for (int n = 0; n < size; n++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
    
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int k = 0; k < padNew; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

void copyBf(BITMAPFILEHEADER in, BITMAPFILEHEADER *out)
{
    out->bfType = in.bfType;
    out->bfSize = in.bfSize;
    out->bfReserved1 = in.bfReserved1;
    out->bfReserved2 = in.bfReserved2;
    out->bfOffBits = in.bfOffBits;
}

void copyBi(BITMAPINFOHEADER in, BITMAPINFOHEADER *out)
{
    out->biSize = in.biSize;
    out->biWidth = in.biWidth;
    out->biHeight = in.biHeight;
    out->biPlanes = in.biPlanes;
    out->biBitCount = in.biBitCount;
    out->biCompression = in.biCompression;
    out->biSizeImage = in.biSizeImage;
    out->biXPelsPerMeter = in.biXPelsPerMeter;
    out->biYPelsPerMeter = in.biYPelsPerMeter;
    out->biClrUsed = in.biClrUsed;
    out->biClrImportant = in.biClrImportant;
}