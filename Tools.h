#include <Setup.h>


#ifndef _BMP_STRUCT_
#define _BMP_STRUCT_
typedef struct BmpDataStruct
{
    UINTN offset;
    UINT32 size;
    UINTN height;
    UINTN width;
    address start;
}BmpStruct;
#endif

UINTN calPage(UINTN fileSize);
BmpStruct ParseBmpHeader(address addr);

