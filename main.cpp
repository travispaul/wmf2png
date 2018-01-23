#include <windows.h>
#include <stdio.h>
#include <string>
using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

static unsigned int
usage() {
    fprintf(stderr, "\nUsage:\n  wmf2png.exe INPUTFILE.WMF OUTPUTFILE.PNG\n");
    return 1;
}

static wchar_t *
char2wchar(const char * text) {
    const size_t size = strlen(text) + 1;
    wchar_t *wText = new wchar_t[size];
    mbstowcs(wText, text, size);
    return wText;
}

static int
GetEncoderClsid(const wchar_t * format, CLSID* pClsid) {

    unsigned int num = 0;
    unsigned int size = 0;

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0) {
        fprintf(stderr, "Fatal: GetImageEncodersSize failed\n");
        return -1;
    }

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) {
        fprintf(stderr, "Fatal: malloc failed\n");
        return -1;
    }

    GetImageEncoders(num, size, pImageCodecInfo);

    for (unsigned int i = 0; i < num; ++i) {
        if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[i].Clsid;
            free(pImageCodecInfo);
            return i;
        }
    }

    free(pImageCodecInfo);
    fprintf(stderr, "Fatal: Unable to find encoder for format %ws\n", format);
    return -1;
}


int
main(unsigned const int argc, const char ** argv) {

    GdiplusStartupInput gdiplusStartupInput;
    unsigned long gdiplusToken;
    CLSID encoderClsid;
    Status status;
    int getEncoderResult;

    if (argc < 3) {
        fprintf(stderr, "Error: You must supply an input WMF file and an output PNG file.\n");
        return usage();
    }

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    getEncoderResult = GetEncoderClsid(L"image/png", &encoderClsid);

    if (getEncoderResult >= 0) {
        Metafile input(char2wchar(argv[1]));
        status = input.Save(char2wchar(argv[2]), &encoderClsid, NULL);
        if (status != Ok) {
            fprintf(stderr, "Failed to save transcoded PNG file\n");
        }
    } else {
        fprintf(stderr, "Failed to transcode WMF file\n");
    }

    GdiplusShutdown(gdiplusToken);

    return 0;
}
