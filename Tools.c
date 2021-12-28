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


BmpStruct LoadBmpFileToMemory(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file,
    CHAR16 *filename
)
{
    BmpStruct bmp;
    EFI_FILE_PROTOCOL *target=NULL;
    address addr;
    EFI_STATUS status = OpenFile(file,filename,&target,EFI_FILE_MODE_READ,EFI_FILE_READ_ONLY);
    if(RETRURN_IF_ERROR(status,L"read logo")){
        return bmp;
    }
    UINT64 infoBuffer= 1000;
    EFI_FILE_INFO fileInfo = GetFileInfo(target,&infoBuffer);
    UINT64 fileSize = fileInfo.FileSize;
    status = ReadToMemoryPage(target,&fileSize,&addr);
    if(RETRURN_IF_ERROR(status,L"Read memory by logo")){
        return bmp;
    }
    //读取宽高
    bmp = ParseBmpHeader(addr);
    bmp.size = fileInfo.FileSize;
    // 对角翻转
    status = ReserveMemory(&addr,fileInfo.FileSize,bmp.width,bmp.height,bmp.offset);
    //剧中显示
    bmp.start = addr+bmp.offset;
    if(RETRURN_IF_ERROR(status,L"Read memory by logo")){
        return bmp;
    }
    //设置开始地址
    #ifdef LOG
        Print(L"read start:%d\n",bmp.start);
    #endif //LOG
    return bmp;
}


