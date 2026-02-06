#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBQUAD palette[16];

    HANDLE hIn, hOut;
    DWORD rw;

    char inName[256];
    cout << "Enter 4-bit BMP file name: ";
    cin >> inName;

    hIn = CreateFileA(inName, GENERIC_READ, FILE_SHARE_READ,
                      NULL, OPEN_EXISTING, 0, NULL);
    if (hIn == INVALID_HANDLE_VALUE)
    {
        cout << "Cannot open input file\n";
        return 1;
    }

    hOut = CreateFileA("o.bmp", GENERIC_WRITE, 0,
                       NULL, CREATE_ALWAYS, 0, NULL);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        CloseHandle(hIn);
        cout << "Cannot create output file\n";
        return 1;
    }

    ReadFile(hIn, &bf, sizeof(bf), &rw, NULL);
    ReadFile(hIn, &bi, sizeof(bi), &rw, NULL);

    cout << "Width:  " << bi.biWidth << endl;
    cout << "Height: " << bi.biHeight << endl;
    cout << "Bits:   " << bi.biBitCount << endl;

    if (bi.biBitCount != 4)
    {
        cout << "This program works only with 4-bit BMP\n";
        CloseHandle(hIn);
        CloseHandle(hOut);
        return 1;
    }

    ReadFile(hIn, palette, 16 * sizeof(RGBQUAD), &rw, NULL);

    int width = bi.biWidth;
    int height = bi.biHeight;

    int inRowSize  = ((width + 1) / 2 + 3) & ~3; // 4 bpp
    int outRowSize = (width * 2 + 3) & ~3;       // 16 bpp

    BYTE* inRow = new BYTE[inRowSize];
    WORD* outRow = new WORD[width];

    BITMAPFILEHEADER bfOut = bf;
    BITMAPINFOHEADER biOut = bi;

    bfOut.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    biOut.biBitCount = 16;
    biOut.biCompression = BI_RGB;
    biOut.biSizeImage = outRowSize * height;
    bfOut.bfSize = bfOut.bfOffBits + biOut.biSizeImage;

    WriteFile(hOut, &bfOut, sizeof(bfOut), &rw, NULL);
    WriteFile(hOut, &biOut, sizeof(biOut), &rw, NULL);
    
    for (int y = 0; y < height; y++)
    {
        ReadFile(hIn, inRow, inRowSize, &rw, NULL);

        int px = 0;
        for (int i = 0; i < (width + 1) / 2; i++)
        {
            BYTE b = inRow[i];

            BYTE idx1 = (b >> 4) & 0x0F;
            BYTE idx2 = b & 0x0F;

            RGBQUAD c = palette[idx1];
            WORD r = (c.rgbRed   >> 3) & 0x1F;
            WORD g = (c.rgbGreen >> 3) & 0x1F;
            WORD bl = (c.rgbBlue >> 3) & 0x1F;

            outRow[px++] = (r << 10) | (g << 5) | bl;

            if (px < width)
            {
                c = palette[idx2];
                r = (c.rgbRed   >> 3) & 0x1F;
                g = (c.rgbGreen >> 3) & 0x1F;
                bl = (c.rgbBlue >> 3) & 0x1F;
                outRow[px++] = (r << 10) | (g << 5) | bl;
            }
        }

        WriteFile(hOut, outRow, width * 2, &rw, NULL);

        BYTE padding[4] = {0};
        WriteFile(hOut, padding, outRowSize - width * 2, &rw, NULL);
    }

    delete[] inRow;
    delete[] outRow;

    CloseHandle(hIn);
    CloseHandle(hOut);

    cout << "Conversion 4 -> 16 completed successfully\n";
    system("pause");
    return 0;
}
