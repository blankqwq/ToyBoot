#include "Setup.h"
#include "Tools.h"
#include <File.h>
#include <MemoryUefi.h>
UINTN GetValue(
    IN EFI_PHYSICAL_ADDRESS StartAddress,
    IN UINTN Offset,
    IN UINTN Size
)
{
    UINT8 *ByteStart = (UINT8 *)StartAddress + Offset;
    UINTN Result = 0;
    for(UINTN i = 0; i < Size; i++)
    {
        Result += *(ByteStart + i) << i * 8;
    }
    return Result;
}

BmpStruct ParseBmpHeader(address addr){
    //读取对应内存地址
    BmpStruct bmp;
    bmp.width = GetValue(addr,0x12,4);
    bmp.height = GetValue(addr,0x16,4);
    //default
    bmp.offset = 54;
    #ifdef LOG
        Print(L"get data:%X",GetValue(addr,0,2));
        Print(L"read data = height:%d,width:%d\n",bmp.height,bmp.width);
    #endif
    return bmp;
}


UINT64 calPage(UINT64 fileSize){
    return fileSize / (4 * 1024) + 1;
}

