#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBQUAD palette[16];

    HANDLE hIn, hOut1, hOut2;
    DWORD rw;

    char inName[256];
    cout << "Enter 4-bit BMP file name: ";
    cin >> inName;

    int threshold;
    cout << "Enter threshold for binarization (0-255): ";
    cin >> threshold;
    if (threshold < 0) threshold = 0;
    if (threshold > 255) threshold = 255;

    hIn = CreateFileA(inName, GENERIC_READ, FILE_SHARE_READ,
                      NULL, OPEN_EXISTING, 0, NULL);
    if (hIn == INVALID_HANDLE_VALUE)
    {
        cout << "Cannot open input file\n";
        return 1;
    }

    hOut1 = CreateFileA("o1.bmp", GENERIC_WRITE, 0,
                       NULL, CREATE_ALWAYS, 0, NULL);
    if (hOut1 == INVALID_HANDLE_VALUE)
    {
        CloseHandle(hIn);
        cout << "Cannot create output file o1.bmp\n";
        return 1;
    }

    hOut2 = CreateFileA("o2.bmp", GENERIC_WRITE, 0,
                       NULL, CREATE_ALWAYS, 0, NULL);
    if (hOut2 == INVALID_HANDLE_VALUE)
    {
        CloseHandle(hIn);
        CloseHandle(hOut1);
        cout << "Cannot create output file o2.bmp\n";
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
        CloseHandle(hOut1);
        CloseHandle(hOut2);
        return 1;
    }

    ReadFile(hIn, palette, 16 * sizeof(RGBQUAD), &rw, NULL);

    int width = bi.biWidth;
    int height = bi.biHeight;

    int inRowSize  = ((width + 1) / 2 + 3) & ~3;
    int outRowSize = (width * 2 + 3) & ~3;

    BYTE* inRow = new BYTE[inRowSize];
    WORD* outRow1 = new WORD[width];
    WORD* outRow2 = new WORD[width];

    BITMAPFILEHEADER bfOut = bf;
    BITMAPINFOHEADER biOut = bi;

    bfOut.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    biOut.biBitCount = 16;
    biOut.biCompression = BI_RGB;
    biOut.biSizeImage = outRowSize * height;
    bfOut.bfSize = bfOut.bfOffBits + biOut.biSizeImage;

    WriteFile(hOut1, &bfOut, sizeof(bfOut), &rw, NULL);
    WriteFile(hOut1, &biOut, sizeof(biOut), &rw, NULL);

    WriteFile(hOut2, &bfOut, sizeof(bfOut), &rw, NULL);
    WriteFile(hOut2, &biOut, sizeof(biOut), &rw, NULL);

    SetFilePointer(hIn, bf.bfOffBits, NULL, FILE_BEGIN);

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

            WORD r1 = (c.rgbRed   >> 3) & 0x1F;
            WORD g1 = (c.rgbGreen >> 3) & 0x1F;
            WORD bl1 = (c.rgbBlue >> 3) & 0x1F;
            outRow1[px] = (r1 << 10) | (g1 << 5) | bl1;

            int brightness = (c.rgbRed * 299 + c.rgbGreen * 587 + c.rgbBlue * 114) / 1000;
            if (brightness > threshold) {
                outRow2[px] = (31 << 10) | (31 << 5) | 31;
            } else {
                outRow2[px] = 0;
            }

            px++;

            if (px < width)
            {
                c = palette[idx2];

                r1 = (c.rgbRed   >> 3) & 0x1F;
                g1 = (c.rgbGreen >> 3) & 0x1F;
                bl1 = (c.rgbBlue >> 3) & 0x1F;
                outRow1[px] = (r1 << 10) | (g1 << 5) | bl1;

                brightness = (c.rgbRed * 299 + c.rgbGreen * 587 + c.rgbBlue * 114) / 1000;
                if (brightness > threshold) {
                    outRow2[px] = (31 << 10) | (31 << 5) | 31;
                } else {
                    outRow2[px] = 0;
                }

                px++;
            }
        }

        WriteFile(hOut1, outRow1, width * 2, &rw, NULL);
        WriteFile(hOut2, outRow2, width * 2, &rw, NULL);

        BYTE padding[4] = {0};
        WriteFile(hOut1, padding, outRowSize - width * 2, &rw, NULL);
        WriteFile(hOut2, padding, outRowSize - width * 2, &rw, NULL);
    }

    delete[] inRow;
    delete[] outRow1;
    delete[] outRow2;


    CloseHandle(hIn);
    CloseHandle(hOut1);
    CloseHandle(hOut2);

    cout << "\nConversion completed successfully!\n";
    cout << "o1.bmp - simple 4->16 bit conversion\n";
    cout << "o2.bmp - 4->16 bit conversion with binarization (threshold="
         << threshold << ")\n";

    system("pause");
    return 0;
}