#include <windows.h>
#include <stdio.h>
#include <string>
using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

static void
printStatusError(Status status) {
    switch (status) {
    case GenericError:
        fprintf(stderr, "Error: GenericError\n");
        break;
    case InvalidParameter:
        fprintf(stderr, "Error: InvalidParameter\n");
        break;
    case OutOfMemory:
        fprintf(stderr, "Error: OutOfMemory\n");
        break;
    case ObjectBusy:
        fprintf(stderr, "Error: ObjectBusy\n");
        break;
    case InsufficientBuffer:
        fprintf(stderr, "Error: InsufficientBuffer\n");
        break;
    case NotImplemented:
        fprintf(stderr, "Error: NotImplemented\n");
        break;
    case Win32Error:
        fprintf(stderr, "Error: Win32Error\n");
        break;
    case WrongState:
        fprintf(stderr, "Error: WrongState\n");
        break;
    case Aborted:
        fprintf(stderr, "Error: Aborted\n");
        break;
    case FileNotFound:
        fprintf(stderr, "Error: FileNotFound\n");
        break;
    case ValueOverflow:
        fprintf(stderr, "Error: ValueOverflow\n");
        break;
    case AccessDenied:
        fprintf(stderr, "Error: AccessDenied\n");
        break;
    case UnknownImageFormat:
        fprintf(stderr, "Error: UnknownImageFormat\n");
        break;
    case FontFamilyNotFound:
        fprintf(stderr, "Error: FontFamilyNotFound\n");
        break;
    case FontStyleNotFound:
        fprintf(stderr, "Error: FontStyleNotFound\n");
        break;
    case NotTrueTypeFont:
        fprintf(stderr, "Error: NotTrueTypeFont\n");
        break;
    case UnsupportedGdiplusVersion:
        fprintf(stderr, "Error: UnsupportedGdiplusVersion\n");
        break;
    case GdiplusNotInitialized:
        fprintf(stderr, "Error: GdiplusNotInitialized\n");
        break;
    case PropertyNotFound:
        fprintf(stderr, "Error: PropertyNotFound\n");
        break;
    case PropertyNotSupported:
        fprintf(stderr, "Error: PropertyNotSupported\n");
        break;
    default:
        fprintf(stderr, "Error: Unknown Error\n");
    }
}

static unsigned int
usage() {
    fprintf(stderr, "\nUsage:\n  wmf2png.exe INPUTFILE.WMF OUTPUTFILE.PNG\n");
    return 1;
}

static wchar_t *
char2wchar(const char * text) {
    const size_t size = strlen(text) + 1;
    wchar_t *wText = new wchar_t[size];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wText, size, text, _TRUNCATE);
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
main(int argc, char ** argv) {
    GdiplusStartupInput gdiplusStartupInput;
    unsigned long gdiplusToken;
    CLSID encoderClsid;
    Status status;
    int getEncoderResult;
    int exitCode = 0;

    if (argc < 3) {
        fprintf(stderr, "Error: You must supply an input WMF file and an output PNG file.\n");
        return usage();
    }

    status = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    if (status != Ok) {
        fprintf(stderr, "GdiplusStartup failed\n");
        printStatusError(status);
        return 1;
    }

    getEncoderResult = GetEncoderClsid(L"image/png", &encoderClsid);

    if (getEncoderResult >= 0) {
        Metafile input(char2wchar(argv[1]));
        status = input.Save(char2wchar(argv[2]), &encoderClsid, NULL);
        if (status != Ok) {
            fprintf(stderr, "Failed to save transcoded PNG file\n");
            printStatusError(status);
            exitCode = 2;
        }
    } else {
        fprintf(stderr, "Failed to transcode WMF file\n");
        exitCode = 3;
    }

    GdiplusShutdown(gdiplusToken);

    return exitCode;
}