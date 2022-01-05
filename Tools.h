#include <Setup.h>



#ifndef _TOOLS_DEFINE
#define _TOOLS_DEFINE
typedef struct BmpDataStruct
{
    UINTN offset;
    UINT32 size;
    UINTN height;
    UINTN width;
    address start;
}BmpStruct;
UINTN calPage(UINTN fileSize);
BmpStruct ParseBmpHeader(address addr);
#endif //_TOOLS_DEFINE


