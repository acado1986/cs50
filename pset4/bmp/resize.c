/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resize an image by a factor of [n]
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }
    
    // remenber the resize factor
    int resize = atoi(argv[1]);

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
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
    
    // store the original dimensions of the bitmap for reading the input file 
    int infile_biWidth = bi.biWidth;
    int infile_biHeight = abs(bi.biHeight);
    
    // determine infile and outfile padding
    int infile_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outfile_padding = (4 - (resize * bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // modify the image dimension for the output file 
    bi.biWidth *= resize;
    bi.biHeight *= resize;
    
    // modify the headers of the output file
    bi.biSizeImage = bi.biWidth * abs(bi.biHeight) * sizeof(RGBTRIPLE) + (abs(bi.biHeight) * outfile_padding);
    bf.bfSize = bf.bfOffBits + bi.biSizeImage;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
   
    // iterate over infile's scanlines
    for (int i = 0; i < infile_biHeight; i++)
    {
        // resize verticaly
        for (int k = 0; k < resize; k++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < infile_biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // write RGB triple to outfile
                for (int m = 0; m < resize; m++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
    
            // add padding to the output file
            for (int k = 0; k < outfile_padding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // set cursor to the beginning of the same scanline
            if (k != resize - 1)
                fseek(inptr, -(infile_biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
                
            // set cursor to the beginning of the next scanline
            else
                fseek(inptr, infile_padding, SEEK_CUR); 
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
