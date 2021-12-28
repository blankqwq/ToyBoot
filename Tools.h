#include <Uefi.h>

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
BmpStruct LoadBmpFileToMemory(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    CHAR16 *filename
);

